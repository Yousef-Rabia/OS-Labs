#! /bin/bash
echo "Yousef Rabia"
rm -r Lab1
mkdir Lab1
cp words.txt ./Lab1/words.txt
cp numbers.txt ./Lab1/numbers.txt
cd Lab1/
paste words.txt numbers.txt > MergedContent.txt
sort MergedContent.txt > SortedMergedContent.txt 
echo "The sorted file is :"
cat SortedMergedContent.txt
chmod 333 SortedMergedContent.txt
sort MergedContent.txt | uniq
tr < SortedMergedContent.txt a-z A-Z 
if [ $? -ne 0 ]; then
    echo  "Permission denied. Can't read file I will solve it"
fi
chmod 777 SortedMergedContent.txt
tr < SortedMergedContent.txt a-z A-Z 
grep -n "^w.*[0-9]" MergedContent.txt 
tr < MergedContent.txt i o >NewMergedContent.txt
paste MergedContent.txt NewMergedContent.txt
