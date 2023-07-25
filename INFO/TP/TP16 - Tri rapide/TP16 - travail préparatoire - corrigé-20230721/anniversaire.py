import sys
import csv


argc = len(sys.argv)
if (argc != 3):
    exit()

path = sys.argv[1]
month = int(sys.argv[2])

def get_birthday(path, month):
    with open(path, newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        names = []
        for line in reader:
            if (len(line) > 2):
                date_of_birth = line[2].split('/')
                if (int(date_of_birth[1]) == month):
                    names.append(line[0] + " " + line[1] + ", le " + date_of_birth[0])
        return names


names = get_birthday(path, month)
print(names)

