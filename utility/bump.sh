#!/bin/bash
FILES="`ls *.png *.jpg 2>/dev/null`"
if [ "$FILES" == "" ]; then
	echo "No files found to convert!"
	exit -1
fi

IFS=$'\n'
for FILE in $FILES; do
	NEW_FILE="${FILE%.*}.bmp"
	echo "$FILE -> $NEW_FILE"
	convert "$FILE" "$NEW_FILE"
	if [ $? -eq 0 ]; then
		rm "$FILE"
	else
		echo "There was an issue converting $FILE"
	fi
done

echo "Done with the conversions; enjoy your new images!"
