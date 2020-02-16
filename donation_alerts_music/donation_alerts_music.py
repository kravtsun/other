#!/usr/bin/env python3

# get page
# get list of current tracks - name + link

import urllib.request as request
from bs4 import BeautifulSoup
import datetime
import sys
# import os
import time
import logging

SLEEP_SECONDS = 100
logging.getLogger().setLevel(logging.DEBUG)


def load_new_tracks(streamer, all_tracks):
    URL = 'https://donationalerts.com/r/{}'.format(streamer)
    logging.debug("Getting page: {} ...".format(URL))
    page = request.urlopen(URL)
    logging.debug("Getting page: {} done".format(URL))
    if page.code != 200:
        print('WARNING: code {} for page {}'.format(page.code, URL))
        return {}
    assert page is not None
    html = page.read()

    # can be necessary to install html5lib?
    logging.debug("Putting page into bs4 parser ...")
    soup = BeautifulSoup(html, 'html.parser', from_encoding='utf-8')
    logging.debug("Putting page into bs4 parser done")
    medialists = soup.find_all('ol', class_='s-last-media-list')

    new_tracks = {}
    for ml in medialists:
        for li in ml.find_all('li'):
            a = li.find('a')
            if a is None:
                print('ERROR: not found link for li: ', li)
            else:
                href = a['href']
                contents = a.contents
                data = '!'.join(contents)
                if href not in all_tracks:
                    new_tracks[href] = data
                all_tracks[href] = data
    return new_tracks


def streamer_filename(streamer):
    return '{}_tracks.txt'.format(streamer)


if __name__ == '__main__':
    all_tracks = {}
    streamers = sys.argv[1:]
    print("start time: {}".format(datetime.datetime.now()))
    print("streamers=,", streamers)
    for streamer in streamers:
        all_tracks[streamer] = {}
        filename = streamer_filename(streamer)
        try:
            with open(filename, 'r', encoding='utf-8') as f:
                for line in f.readlines():
                    columns = line.strip().split(',')
                    href = columns[1]
                    name = ','.join(columns[2:])
                    all_tracks[streamer][href] = name
        except FileNotFoundError:
            print("File not found: {}".format(filename))
            continue

    while True:
        for streamer in streamers:
            logging.debug("Working with streamer: {} ...".format(streamer))
            new_tracks = load_new_tracks(streamer, all_tracks[streamer])
            filename = streamer_filename(streamer)
            now = str(datetime.datetime.now())
            with open(filename, 'a+', encoding='utf-8') as f:
                for href, name in new_tracks.items():
                    track_string = '{},{},{}'.format(now, href, name)
                    print("new_track for {}: {}".format(streamer, track_string))
                    f.write(track_string + '\n')
                    f.flush()
            print(now)

            logging.debug("Working with streamer: {} done, going to sleep for {}".format(streamer, SLEEP_SECONDS))
            time.sleep(SLEEP_SECONDS)
