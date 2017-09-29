import sys
import argparse
import math
from functools import partial
import numpy as np
import cv2

# TODO script with sliders.

# TODO get rid of auxiliary functions called once or twice.

# TODO Size class.

def nparray_from_size(size, channels=1, dtype=np.uint8):
    shape = (size[1], size[0]) if channels == 1 else (size[1], size[0], channels)
    return np.zeros(shape=shape, dtype=dtype)

def size_from_shape(shape):
    return (shape[1], shape[0])

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def get_tuple(self):
        return (self.x, self.y)

    def int_tuple(self):
        return tuple(map(int, self.get_tuple()))

class Segment:
    def __init__(self, pfrom: Point, pto: Point):
        self.pfrom = pfrom
        self.pto = pto

class Chunk:
    def get_segments(self) -> (Point, [Segment]):
        """
        :return pair of ending point and segments in a list - bypassing order.
        """
        return None

    def get_mask(self, size):
        return None

    def get_coeff(self, image: np.ndarray) -> float:
        return None

class SimpleChunk(Chunk):
    def __init__(self, top_left: Point, top_right: Point, bottom_left: Point, bottom_right: Point):
        self.top_left = top_left
        self.top_right = top_right
        self.bottom_left = bottom_left
        self.bottom_right = bottom_right

    def get_segments(self):
        return self.bottom_left, [Segment(self.top_left, self.top_right),
                                  Segment(self.top_right, self.bottom_left),
                                  Segment(self.bottom_left, self.top_left),
                                  Segment(self.top_left, self.bottom_right),
                                  Segment(self.bottom_right, self.bottom_left)]

    def get_points(self):
        return [self.top_left,
                self.top_right,
                self.bottom_right,
                self.bottom_left]

    def get_mask(self, size):
        mask = nparray_from_size(size)
        pts = self.get_points()
        if len(pts) > 0:
            if type(pts[0]) == Point:
                pts = np.array(list(map(Point.get_tuple, pts)), dtype=np.int32)
            cv2.fillPoly(mask, [pts], 255, cv2.LINE_AA)
        return mask

    def get_coeff(self, image):
        size = size_from_shape(image.shape)
        mask = self.get_mask(size)
        roi = mask & image
        # cv2.imshow("roi", roi)
        # cv2.waitKey(0)
        mean_roi_color = float(np.sum(roi)) / np.sum(mask > 0)
        print(mean_roi_color)
        return mean_roi_color


class Circle:
    def __init__(self, center: Point, radius: float):
        self.radius = radius
        self.center = center

    def get_point_from_angle(self, angle: float):
        return Point(self.center.x + self.radius * math.sin(angle),
                     self.center.y + self.radius * math.cos(angle))

# TODO remove debugging method.
def draw_points(mask: np.ndarray, points: [Point], radius=1, color=255):
    for p in points:
        mask = cv2.circle(mask, p.int_tuple(), radius, color, -1)
    cv2.imshow("points", mask)
    cv2.waitKey(0)
    return mask

def draw_patch(mask: np.ndarray, segments: [Segment], power=1, thickness=1) -> None:
    points = []
    for segment in segments:
        points.append(segment.pfrom.int_tuple())
        points.append(segment.pto.int_tuple())
    points = np.array(points, dtype=np.int32)
    hull = cv2.convexHull(points).reshape(-1, 2)
    assert power < 256
    # cv2.imshow('mask', mask)
    # cv2.waitKey(0)
    new_mask = np.zeros(mask.shape, dtype=np.int32)
    cv2.fillPoly(new_mask, [hull], int(power), cv2.LINE_AA)
    mask += new_mask
    # print(np.sum(mask))
    #     cv2.line(mask, segment.pfrom.get_tuple(), segment.pto.get_tuple(), power, thickness, cv2.CV_AA)

def get_circle_from_args(args: argparse.Namespace, margin: float=10) -> Circle:
    width, height = args.size
    center = Point(width / 2.0, height / 2.0)
    radius = min(width / 2.0 - margin, width / 2.0 - margin)
    circle = Circle(center, radius)
    return circle


def run(args):
    assert len(args.size) == 2
    args.size = tuple(args.size)

    alpha_step = 2 * math.pi / args.n
    width, height = args.size
    circle = get_circle_from_args(args)

    # points = []
    # # TODO Visualize points
    # for angle in np.arange(0, math.pi, alpha_step):
    #     points.append(circle.get_point_from_angle(angle))
    # mask = nparray_from_size(args.size, 3)
    # cv2.circle(mask, circle.center.int_tuple(), int(circle.radius), (0, 0, 255), -1)
    # draw_points(mask, points, color=(255, 0, 0), radius=3)
    # return None

    get_point = circle.get_point_from_angle
    # Generate vertical chunks.
    vert_chunks, hor_chunks = [], []

    for angle in np.arange(0, math.pi, alpha_step):
        # points.append(get_point_on_circle(circle, angle))
        next_angle = angle + alpha_step

        vert_chunk = SimpleChunk(get_point(angle),
                                 get_point(next_angle),
                                 get_point(-angle),
                                 get_point(-next_angle))
        vert_chunks.append(vert_chunk)

        hor_chunk = SimpleChunk(get_point(np.pi / 2 + angle),
                                get_point(np.pi / 2 + next_angle),
                                get_point(np.pi / 2 + -angle),
                                get_point(np.pi / 2 + -next_angle))
        hor_chunks.append(hor_chunk)

    # all_chunks = vert_chunks + hor_chunks
    all_chunks = vert_chunks
    image = cv2.imread(args.image, cv2.IMREAD_GRAYSCALE)
    image = cv2.resize(image, args.size)
    cv2.imshow("image", image)
    all_coeffs = [chunk.get_coeff(image) for chunk in all_chunks]
    print(all_coeffs)

    mask = nparray_from_size(args.size, dtype=np.int32)
    for chunk, coeff in zip(all_chunks, all_coeffs):
        start_point, segments = chunk.get_segments()
        draw_patch(mask, segments, power=coeff)
    out_mask = mask.astype(np.float) / np.max(mask)# * 255

    # cv2.normalize(mask, out_mask, 0, 255, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_8U)
    # print(np.max(mask))
    cv2.imshow("chunk_mask", out_mask)
    cv2.waitKey(0)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Visualizing string art from given image")
    parser.add_argument("-i", "--image", type=str, default="lena_crop.png")
    parser.add_argument("--size", default=[1024, 1024], nargs=2, help="canvas size")

    parser.add_argument("-n", type=int, default=100, help="Number of spokes, should be divisible by 4.")
    # parser.add_argument("")

    args = parser.parse_args()
    print(args)

    if args.n % 4 != 0:
        print("ERROR: spokes number should be divisible by 4.")
        print(parser.print_help())
        sys.exit(1)

    run(args)

