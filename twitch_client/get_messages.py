#!/usr/bin/python
import sys, os
import requests
import json
import re

API_URL = 'https://api.twitch.tv/helix/'

# get user-id with https://api.twitch.tv/helix/users?login=<streamer>
STREAMER_USER_ID = '22056283'

JSON_DIR = os.getcwd()

def get_videos():
    client_id = os.environ.get("TWITCH_CLIENT_ID")
    headers = {
        'Content-Type': 'application/json',
        'Accept':'application/json',
        'Client-ID': client_id
    }
    params = {'user_id' : STREAMER_USER_ID, 'first': 100}

    r = requests.get(API_URL + 'videos', params=params, headers=headers)

    data = r.json()['data']

    videos = [d["id"] for d in data]

    return videos

def get_urls(body: str):
    # regexp not for internation format.
    URL_REGEXP = r'((ht|f)tp(s?)\:\/\/[0-9a-zA-Z]([-.\w]*[0-9a-zA-Z])*(:(0-9)*)*(\/?)([a-zA-Z0-9\-\.\?\,\'\/\\\+&amp;%\$#_]*)?)'
    # DOMAIN_REGEXP = 'https?://(?:[-\w.]|(?:%[\da-fA-F]{2}))+'
    urls = [groups[0] for groups in re.findall(URL_REGEXP, body)]
    def is_good_url(url: str):
        for bad_url in ['youtube.com', 'youtu.be', 'donationalerts.com', 'clips.twitch.tv']:
            if bad_url in url:
                return False
        return True
    return list(filter(is_good_url, urls))


if __name__ == '__main__':
    print("stream_id,sender_name,url")
    videos = get_videos()
    for v in videos:
        filepath = "{}/{}.json".format(JSON_DIR, v)
        try:
            with open(filepath, "r") as f:
                j = json.load(f)
            comments = j['comments']
            for c in comments:
                sender = c['commenter']['name']
                body = c['message']['body']
                if sender in ['nightbot']:
                    continue
                urls = get_urls(body)
                for url in urls:
                    print('{},{},{}'.format(v, sender, url))
        except IOError:
            pass
