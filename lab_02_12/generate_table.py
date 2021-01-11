import sys

import random
import time

if len(sys.argv) != 2 or not sys.argv[1].isdigit():
    exit()


def str_time_prop(start, end, format, prop):
    stime = time.mktime(time.strptime(start, format))
    etime = time.mktime(time.strptime(end, format))

    ptime = stime + prop * (etime - stime)

    return time.strftime(format, time.localtime(ptime))


def random_date(start, end, prop):
    return str_time_prop(start, end, '%d;%m;%Y', prop)


male_names = ["Noah", "Liam", "William", "Mason",
              "James", "Benjamin", "Jacob", "Michael",
              "Elijah", "Ethan", "Sam", "Marshall"]

female_names = ["Emma", "Olivia", "Ava", "Sophia",
                "Isabella", "Mia", "Charlotte", "Abigail",
                "Emily", "Harper", "Stella", "Ruby"
                ]

surnames = ["Abramson", "Bosworth", "Daniels", "Donovan",
            "Douglas", "Erickson", "Foster", "Gill"
            "Harrison", "Holmes", "Jenkin", "Nicholson",
            "Roberts", "Sheldon", "Warren", "Wesley"
            ]

groups = ["iu", "mt", "fn", "ibm", "bmt", "sm", "l"]

streets = ["Oxford Street", "Abbey Road", "Brick Lane",
           "Carnaby Street", "Piccadilly", "Crosswall",
           "Houndsditch", "Queenhithe", "Old Jewry"
           ]

with open("./data/data.csv", "w") as f:
    for i in range(int(sys.argv[1])):
        surname = random.choice(surnames)
        avg = "{:.2f}".format(random.uniform(2.0, 100.0))
        age = str(random.randint(16, 45))

        if random.choice([0, 1]) == 1:
            sex = "male"
            name = random.choice(male_names)
        else:
            sex = "female"
            name = random.choice(female_names)

        group = random.choice(groups) + str(random.randint(1, 12)) + \
            "-" + str(random.randint(11, 49)) + random.choice(["B", "M"])

        f.write(surname + ";" + name + ";" + group + ";" +
                sex + ";" + age + ";" + avg + ";" +
                random_date("1;1;1995", "1;1;2020", random.random()) + ";")

        if random.choice([0, 1]) == 1:
            f.write("yes;;;;" +
                    str(random.randint(1, 20)) + ";" +
                    str(random.randint(1, 1000)) + ";")
        else:
            f.write("no;" + random.choice(streets) + ";" +
                    str(random.randint(1, 30)) + ";" +
                    str(random.randint(1, 100)) + ";;;")

        f.write("\n")
