#!/bin/bash
directory=$1

let count1=0
let count2=0
let dot_size=0
let reg_size=0

dot_file=`ls -a | grep '^\.' | sort`
reg_file=`ls | sort`

declare -a dot_array
declare -a reg_array
declare -a final_array

for f in $dot_file
do
 if [ -f $f ]
 then
  dot_array[$dot_size]="$directory/$f"
  let dot_size=dot_size+1
 fi
done


for f in $reg_file
do
 if [ -f $f ]
 then
  reg_array[$reg_size]="$directory/$f"
  let reg_size=reg_size+1
 fi
done


while [ $count1 -lt $dot_size ]
do
 final_array[$count1]=${dot_array[$count1]}
 let count1=count1+1
done

let count2=dot_size
let count3=0
let total=$dot_size+$reg_size
while [ $count2 -lt $total ]
do
 final_array[$count2]=${reg_array[$count3]}
 let count2=count2+1
 let count3=count3+1
done


let count1=0
let count2=0
while [ $count1 -lt $total ]
do
    let count2=count1+1
    if [ $count2 -ne $total ]
    then
	while [ $count2 -lt $total ]
	do
	    cmp -s "${final_array[$count1]}" "${final_array[$count2]}"
	    if [ $? -eq 0 ]
	    then
		echo "same content"
		rm "${final_array[$count2]}"
		ln "${final_array[$count1]}" "${final_array[$count2]}"
	    fi	
	    let count2=count2+1
	done
    fi
    let count1=count1+1
done