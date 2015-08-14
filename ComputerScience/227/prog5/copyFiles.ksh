#!/bin/ksh

#location="/Users/patrick/Classes/227/Programs/prog5"
location="/home/cs227/spring13/prog5"

echo "Copying the BookListClient1 and BookListClient2 files"
cp ${location}/BookListClient1.java .
cp ${location}/BookListClient1.class .
cp ${location}/BookListClient2.java .
cp ${location}/BookListClient2.class .

echo "Copying the title test files"
cp ${location}/test[01][0-9].txt .
cp ${location}/test[01][0-9].commands .

echo "Copying the author test files"
cp ${location}/testA[01][0-9].txt .
cp ${location}/testA[01][0-9].commands .

echo "Copying Patrick's answers to each of the title and author tests" 
cp ${location}/patrickTest[01][0-9].out .
cp ${location}/patrickTest[01][0-9].err .
cp ${location}/patrickTestA[01][0-9].out .
cp ${location}/patrickTestA[01][0-9].err .

echo "Copying the two test scripts"
cp ${location}/test*ksh .
