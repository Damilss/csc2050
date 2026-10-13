#!/bin/bash

# Emilio Scott
# Date created: 2026/09/18
# statistics {-[r]ows|-[c]olumns} [input_file]
# a script that calculates the average and median of either rows or columns of
# whole numbers separated by `\t'

# USAGE string denoted by all capital variable. (2026/09/20). I just learned
# about "readonly" in bash, and what it does. Very cool!
set -euo pipefail

readonly USAGE='Usage: statistics {-rows|-cols} [file]'

# check for correct amount of parameters, if not fails and outputs to stderr
if [[ $# -gt 2 ]] || [[ $# -lt 1 ]]; then
	echo "$USAGE" >&2
	exit 1
fi

# flag is able to become immutable after being set
readonly flag="$1"
filename="${2:-/dev/stdin}"

# check the flag before doing any work. Any option beginning with a lowercase
# r after the dash means rows, and any option beginning with c means columns.
if [[ ! "$flag" =~ ^-r ]] && [[ ! "$flag" =~ ^-c ]]; then
	echo "$USAGE" >&2
	exit 1
fi

# checks if a file was specified and, if so, whether it exists and is readable
if [[ "$#" -eq 2 ]] && { [[ ! -e "$filename" ]] || [[ ! -r "$filename" ]]; }; then
	echo "statistics: cannot read $filename" >&2
	exit 1
fi

# temporary files include the process id so two copies of statistics running
# at the same time do not overwrite each other's temporary data
readonly tmp_values="tmpValues$$"
readonly tmp_input="tmpInput$$"

# removes any temporary files created by this process
cleanup() {
	rm -f "$tmp_values" "$tmp_input"
}

# always clean up normally, and also clean up before exiting on a signal
trap cleanup EXIT
trap 'cleanup; exit 1' HUP INT TERM

# if no input file was given, save standard input in a temporary file. This
# lets the same row and column logic work for both a filename and stdin.
if [[ -z "$filename" ]]; then
	cat > "$tmp_input"
	filename="$tmp_input"
fi

# store the number of integers in one row, which is also the number of columns
# in the input. wc -w works because the values are separated by whitespace.
elements=$(head -n 1 "$filename" | wc -w)

# -r branch
if [[ "$flag" =~ ^-r ]]; then
	# print the heading exactly once before the row statistics. -e makes echo
	# turn \t into a real tab character.
	echo -e "Average\tMedian"

	# while loop iterates through each line of the file
	while IFS= read -r line; do
		sum=0

		# start with an empty temporary file for the values in this row
		: > "$tmp_values"

		for ((i = 1; i <= elements; i++)); do
			# takes the line, splits it at tabs, then gets the field based on
			# the iterator count. cut fields are numbered starting at 1.
			element=$(echo "$line" | cut -d $'\t' -f "$i")

			# add the current element to the running sum, then save it so the
			# row can be numerically sorted when calculating the median
			sum=$((sum + element))
			echo "$element" >> "$tmp_values"
		done

		# integer division rounds down, so adding half the number of values
		# before dividing rounds the average to the nearest whole number.
		# For an exact .5 value, this rounds upward as required.
		average=$(((sum + elements / 2) / elements))

		# median uses the upper middle value when there is an even number of
		# values. This formula gives that 1-based position for odd or even.
		median_index=$((elements / 2 + 1))
		median=$(sort -n "$tmp_values" | head -n "$median_index" | tail -n 1)

		echo -e "$average\t$median"
	done < "$filename"

	exit 0

# -c branch
elif [[ "$flag" =~ ^-c ]]; then
	# a column is gathered into the temporary file, then processed in the
	# same way as a row. Do not assume any fixed number of rows.
	row_count=$(wc -l < "$filename")

	echo "Averages:"

	for ((i = 1; i <= elements; i++)); do
		sum=0

		while IFS= read -r line; do
			element=$(echo "$line" | cut -d $'\t' -f "$i")
			sum=$((sum + element))
		done < "$filename"

		average=$(((sum + row_count / 2) / row_count))

		# keep all averages on one output line. -n stops echo from adding a
		# newline, and every value is followed by a tab, including the last one.
		echo -n -e "$average\t"
	done

	# end the averages line. For an empty file this still prints a blank line.
	echo

	echo "Medians:"
	median_index=$((row_count / 2 + 1))

	for ((i = 1; i <= elements; i++)); do
		: > "$tmp_values"

		while IFS= read -r line; do
			element=$(echo "$line" | cut -d $'\t' -f "$i")
			echo "$element" >> "$tmp_values"
		done < "$filename"

		median=$(sort -n "$tmp_values" | head -n "$median_index" | tail -n 1)

		# keep all medians on one output line, the same way as the averages
		echo -n -e "$median\t"
	done

	# end the medians line
	echo

	exit 0
fi

# this point should never be reached because arguments and flags are checked
# above, but leave a final error path in case the script is changed later
echo 'An undefined error occurred' >&2
exit 1
