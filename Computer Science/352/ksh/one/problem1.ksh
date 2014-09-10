#!/bin/ksh
ls --reverse --format=single-column /home/cs352/fall14/books
echo
cat /home/cs352/fall14/books/mohicans.txt | fgrep "Chingachgook"
echo
cat /home/cs352/fall14/books/mohicans.txt | fgrep "Hawkeye" | fgrep "Uncas"
echo
cat /etc/passwd | fgrep "bin/csh" | wc -l
echo
echo -n "Words in the first 200 lines: "; head -200 /home/cs352/fall14/books/mobydick.txt | wc -w; echo D\'Artagnan and Athos:; cat /home/cs352/fall14/books/musketeers.txt | fgrep "Athos" | fgrep "D'Artagnan"; echo "Gettysburg"; head -11 /home/cs352/fall14/books/gettysburg.txt | tr [:lower:] [:upper:]; tail -n +12 /home/cs352/fall14/books/gettysburg.txt | tr [:upper:] [:lower:]
