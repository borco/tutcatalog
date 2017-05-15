#! /usr/bin/python3

import argparse
import sqlite3
import os
import io
import datetime

image1 = "image1.png"
image2 = "image2.png"

text1 = """[image](image1.png)

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
[image](image1.png)

## INFO 3

* Lorem ipsum dolor sit amet, consectetuer adipiscing elit.
* Aenean commodo ligula eget dolor.
* Aenean massa.
* Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.
* Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem.
* Nulla consequat massa quis enim.
* Donec pede justo, fringilla vel, aliquet nec, vulputate 

[image](image2.png)

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

def create_database(args):
    print("generating {} tutorials in {}, using tutorial name prefix {}".format(args.count, args.file, args.title))

    conn = sqlite3.connect(args.file)
    c = conn.cursor()
    c.execute("DROP TABLE IF EXISTS tutorials")
    c.execute("DROP TABLE IF EXISTS infos")
    c.execute("CREATE TABLE tutorials (id INTEGER PRIMARY KEY, title TEXT, publisher TEXT, authors TEXT, has_info BOOLEAN, has_checksum BOOLEAN, todo BOOLEAN, keep BOOLEAN, complete BOOLEAN, rating INTEGER, viewed BOOLEAN, deleted BOOLEAN, online BOOLEAN, no_backup BOOLEAN, duration TEXT, size INTEGER, path TEXT, levels TEXT, created DATE, released DATE, modified DATE, learning_paths TEXT, tags TEXT, extra_tags TEXT)")
    c.execute("CREATE TABLE infos (id INTEGER PRIMARY KEY, tutorial_id INTEGER, name TEXT, data BLOB)")
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
        size = i * i * 100 % (1024 * 1024 * 1024 * 100)
        no_backup = i % 5 == 0
        parent_path = parent_paths[i % len(parent_paths)]
        skip_path = skip_paths[i % len(skip_paths)]
        path = (skip_path if no_backup else parent_path) + "/" + (title if i % 10 != 9 else title + "__")
        level = levels[i % len(levels)]
        created = datetime.datetime.now() - datetime.timedelta(minutes = i)
        released = datetime.datetime.now() - datetime.timedelta(hours = i)
        modified = datetime.datetime.now() + datetime.timedelta(minutes = i)
        lps = learning_paths[i % len(learning_paths)]
        ts = tags[i % len(tags)]
        ets = extra_tags[i % len(extra_tags)]

        c.execute("INSERT INTO tutorials (title, publisher, authors, has_info, has_checksum, todo, keep, complete, rating, viewed, deleted, online, no_backup, duration, size, path, levels, created, released, modified, learning_paths, tags, extra_tags) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
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
                ]
                )

        if has_info:
            tutorial_id = c.lastrowid

            f = files[i % len(files)]
            c.execute("INSERT INTO infos (tutorial_id, name, data) VALUES (?, ?, ?)", [ tutorial_id, "info.tc",  "#" + title + "\n\n" + f.info ])

            for file_name in f.images:
                with open(file_name, 'rb') as input_file:
                    blob = input_file.read()
                    c.execute("INSERT INTO infos (tutorial_id, name, data) VALUES (?, ?, ?)", [ tutorial_id, file_name,  sqlite3.Binary(blob) ])

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
