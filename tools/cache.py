#! /usr/bin/python3

import argparse
import sqlite3
import os
import io
import datetime
import binascii

image1 = "image1.png"
image2 = "image2.png"

text1 = """![image](image1.png)

## INFO 1
 
Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate
"""

text2 = """
## INFO 2

### Foo

Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et

###Bar

Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et """

text3 = """
![image](image1.png)

## INFO 3

* Lorem ipsum dolor sit amet, consectetuer adipiscing elit.
* Aenean commodo ligula eget dolor.
* Aenean massa.
* Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.
* Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem.
* Nulla consequat massa quis enim.
* Donec pede justo, fringilla vel, aliquet nec, vulputate 

![image](image2.png)

Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor.

Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.

Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate
"""

class Files:
    def __init__(self, info, images = []):
        self.info = info
        self.images = images

files = [
    Files(text1, [image1]),
    Files(text2),
    Files(text3, [image1, image2])
]

parent_paths = [
    "c:\\_TUTORIALS\\FOO",
    "c:\\_TUTORIALS\\BAR",
]

skip_paths = [
    "c:\\_TUTORIALS_SKIP\\FOO",
    "c:\\_TUTORIALS_SKIP\\BAR",
]

levels = [
    "beginner",
    "intermediate",
    "avanced",
    "beginner, intermediate",
    "intermediate, advanced",
    "beginner, intermediate, advanced",
    None,
    None,
    None,
    None,
    None,
]

learning_paths = [
    "path1",
    "path2",
    "path1, path2",
    None,
    None,
    None,
    None,
    None,
    None,
    None,
    None,
    None,
    None,
    None,
    None,
]

tags = [
    "tag1",
    "tag2",
    "tag3",
    "tag4",
    "tag5",
    "tag1, tag2",
    "tag1, tag2, tag3",
    None,
    None,
    None,
    None,
    None,
    None,
    None,
    None,
    None,
    None,
    None,
]

extra_tags = [
    "etag1",
    "etag2",
    "etag3",
    "etag4",
    "etag5",
    "etag1, etag2",
    "etag1, etag2, etag3",
    None,
    None,
    None,
    None,
]

urls = [
    "http://www.google.com",
    "http://www.youtube.com",
    "http://www.udemy.com",
    None,
    None,
]

released_dates = [
    "2017",
    "2017/01",
    "2017/10/20",
    None,
    None,
]
def crcFile(name):
    with open(name, 'rb') as f:
        data = f.read()
        return crc(data)
    return ""

def crcData(data):
    crc = binascii.crc32(data)
    return format(crc, 'x')

def create_database(args):
    print("generating {} tutorials in {}, using tutorial name prefix {}".format(args.count, args.file, args.title))

    conn = sqlite3.connect(args.file)
    c = conn.cursor()
    c.execute("DROP TABLE IF EXISTS tutorials")
    c.execute("DROP TABLE IF EXISTS infos")
    c.execute("DROP TABLE IF EXISTS images")
    c.execute("DROP TABLE IF EXISTS files")
    c.execute("CREATE TABLE tutorials (id INTEGER PRIMARY KEY, title TEXT, publisher TEXT, authors TEXT, has_info BOOLEAN, has_checksum BOOLEAN, todo BOOLEAN, keep BOOLEAN, complete BOOLEAN, rating INTEGER, viewed BOOLEAN, deleted BOOLEAN, online BOOLEAN, no_backup BOOLEAN, duration TEXT, size INTEGER, path TEXT, levels TEXT, created DATE, released TEXT, modified DATE, learning_paths TEXT, tags TEXT, extra_tags TEXT, url TEXT)")
    c.execute("CREATE TABLE infos (id INTEGER PRIMARY KEY, tutorial_id INTEGER UNIQUE, name TEXT, modified DATE, checksum TEXT, data BLOB)")
    c.execute("CREATE TABLE images (id INTEGER PRIMARY KEY, tutorial_id INTEGER, name TEXT, modified DATE, checksum TEXT, data BLOB)")
    c.execute("CREATE TABLE files (id INTEGER PRIMARY KEY, tutorial_id INTEGER UNIQUE, name TEXT, modified DATE, checksum TEXT, data BLOB)")
    for i in range(args.count):
        title = "{} {}".format(args.title, i)
        publisher = "Publisher {}".format(i % 3)
        authors = "Author {}".format(i) if i % 5 == 0 else "Author {}, Author {}".format(i % 5, i % 5 + 1)
        
        has_info = i % 10 != 9
        has_checksum = i % 3 == 0
        todo = i % 15 == 0
        keep = i % 4 == 0
        complete = i % 20 != 0
        rating = i % 10 - 5
        viewed = i % 4 == 0
        deleted = i % 6 == 0
        online = i % 7 == 0
        duration = i + 10
        url = urls[i % len(urls)]
        size = i * i * 100 % (1024 * 1024 * 1024 * 100)
        no_backup = i % 5 == 0
        parent_path = parent_paths[i % len(parent_paths)]
        skip_path = skip_paths[i % len(skip_paths)]
        path = (skip_path if no_backup else parent_path) + "/" + (title if i % 5 != 4 else title + "__")
        level = levels[i % len(levels)]
        created = datetime.datetime.now() - datetime.timedelta(minutes = i)
        released = released_dates[i % len(released_dates)]
        modified = datetime.datetime.now() + datetime.timedelta(minutes = i)
        lps = learning_paths[i % len(learning_paths)]
        ts = tags[i % len(tags)]
        ets = extra_tags[i % len(extra_tags)]

        c.execute("INSERT INTO tutorials (title, publisher, authors, has_info, has_checksum, todo, keep, complete, rating, viewed, deleted, online, no_backup, duration, size, path, levels, created, released, modified, learning_paths, tags, extra_tags, url) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
            [
                title,
                publisher,
                authors,
                has_info,
                has_checksum,
                todo,
                keep,
                complete,
                rating,
                viewed,
                deleted,
                online,
                no_backup,
                duration,
                size,
                path,
                level,
                created,
                released,
                modified,
                lps,
                ts,
                ets,
                url,
                ]
                )

        if has_info:
            tutorial_id = c.lastrowid

            f = files[i % len(files)]
            info_name = "info.tc"
            info_data = "#" + title + "\n\n" + f.info
            info_checksum = crcData(info_data.encode())
            info_modified = modified
            c.execute("INSERT INTO infos (tutorial_id, name, data, modified, checksum) VALUES (?, ?, ?, ?, ?)", [ tutorial_id,  info_name, info_data, info_modified, info_checksum])

            for image_name in f.images:
                image_modified = datetime.datetime.fromtimestamp(os.path.getmtime(image_name))
                with open(image_name, 'rb') as image_file:
                    image_data = image_file.read()
                    image_checksum = crcData(image_data)
                    c.execute("INSERT INTO images (tutorial_id, name, data, modified, checksum) VALUES (?, ?, ?, ?, ?)", [ tutorial_id, image_name,  sqlite3.Binary(image_data), image_modified, image_checksum ])

    conn.commit()
    conn.close()

if __name__ == "__main__":
    default_file = "c:/sources/tutcatalog-dummy-cache.sqlite" # tutcatalog cache
    default_count = 100
    default_title = "Title"
    parser = argparse.ArgumentParser()
    parser.add_argument("-c", "--count", help="number of tutorials to generate (default: {})".format(default_count), type=int, default=default_count, metavar="N")
    parser.add_argument("-f", "--file", help="output file (default: {})".format(default_file), default=default_file)
    parser.add_argument("-t", "--title", help="tutorial title prefix (default: {})".format(default_title), default=default_title)
    args = parser.parse_args()
    os.chdir(os.path.dirname(__file__))
    print("current dir: {}".format(os.getcwd()))
    create_database(args)
