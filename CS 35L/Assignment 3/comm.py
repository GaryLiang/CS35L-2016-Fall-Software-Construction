#!/usr/bin/python

import sys
from optparse import OptionParser

def main():
    version_msg = "$prog 1.0"

    usage_msg = "$prog [OPTION] FILE1 FILE2"
    parser = OptionParser(version = version_msg,usage = usage_msg)

    parser.add_option("-1",action = "store_true", dest = "first", default =0)
    parser.add_option("-2",action = "store_true", dest = "second", default =0)
    parser.add_option("-3",action = "store_true", dest = "third", default =0)
    parser.add_option("-u","--unsorted",action = "store_true", dest = "unsorted", default =0)
    
    options, args = parser.parse_args(sys.argv[1:])

    content1 = []
    content2 = []

    if len(args) != 2:
        parser.error("Number of operands does not match")

    if args[0] == "-":
        content1 = sys.stdin.readlines()
    else:
        file1 = args[0]
        try:
            file1 = open(file1)
            content1 = file1.readlines()
            file1.close()
        except:
            parser.error("Read file failed")

    if args[1] == "-":
        content2 = sys.stdin.readlines()
    else:
        file2 = args[1]
        try:
            file2 = open(file2)
            content2 = file2.readlines()
            file2.close()
        except:
            parser.error("Read file failed")



    first = options.first
    second = options.second
    third = options.third
    unsorted = options.unsorted

    if unsorted == False:
        i = 0
        j = 0
        while i < len(content1):
            while j < len(content2):
                if content1[i] == content2[j]:
                    if third == False:
                        sys.stdout.write("\t\t" + content1[i])
                    j = j + 1
                    i = i + 1
                    break
                elif content1[i] <= content2[j]:
                    if first == False:
                        sys.stdout.write(content1[i])
                    i = i + 1
                    break
                elif content1[i] >= content2[j]:
                    if second == False:
                        sys.stdout.write("\t" + content2[j])
                    j = j+1
            if j == len(content2):
                break

        while i < len(content1):
            if first == False:
                sys.stdout.write(content1[i])
            i = i +1
        
        while j <len(content2):
            if second == False:
                sys.stdout.write("\t" + content2[j])
            j = j + 1
    else:
        i = 0
        j = 0
        found = False
        while i < len(content1):
            while j < len(content2):
                if content1[i] == content2[j]:
                    sys.stdout.write("\t\t"+content1[i])
                    found = True
                    j = j + 1
                else:
                    j = j + 1
            if found == False:
                sys.stdout.write(content1[i])
            found = False
            i = i + 1
            j = 0

        i =0
        while i < len(content2):
            while j < len(content1):
                if content1[j] == content2[i]:
                    found = True
                j = j + 1
            if found == False:
                sys.stdout.write("\t"+content2[i])
            found == False
            i = i + 1
            j = 0
if __name__ == "__main__":
    main()
