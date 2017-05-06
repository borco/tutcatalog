#! /usr/bin/python3

import argparse
import sqlite3
import os

def create_database(args):
    print("generating {} tutorials in {}".format(args.tutorials, args.file))

    conn = sqlite3.connect(args.file)
    c = conn.cursor()
    c.execute("DROP TABLE IF EXISTS tutorials")
    c.execute("CREATE TABLE tutorials (id INTEGER PRIMARY KEY, title, publisher, authors)")
    for i in range(args.tutorials):
        title = "Title {}".format(i + 1)
        publisher = "Publisher {}".format(i % 3 + 1)
        authors = "Author {}".format(i + 1) if i % 5 == 0 else "Author {}, Author {}".format(i % 5, i % 5 + 1)
        c.execute("INSERT INTO tutorials (title, publisher, authors) VALUES ('{}', '{}', '{}')".format(title, publisher, authors))
    conn.commit()
    conn.close()

if __name__ == "__main__":
    default_file = "c:/sources/dummy.sqlite" # tutcatalog cache
    default_tutorials = 10000
    parser = argparse.ArgumentParser()
    parser.add_argument("-t", "--tutorials", help="number of tutorials to generate (default: {})".format(default_tutorials), type=int, default=default_tutorials, metavar="N")
    parser.add_argument("-f", "--file", help="output file (default: {})".format(default_file), default=default_file)
    args = parser.parse_args()
    create_database(args)
