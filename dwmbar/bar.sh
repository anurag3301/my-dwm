#!/bin/bash

# Copyright 2019 Archie Hilton <archie.hilton1@gmail.com>

# This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
#
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with this program.  If not, see <https://www.gnu.org/licenses/>.
if [[ ! -z $CACHE_DIR ]]; then
	OUTPUT_CACHE="$CACHE_DIR"
else
	OUTPUT_CACHE="/home/$USER/.config/dwmbar/.cache/"
fi

source "$CONFIG_FILE" 2>/dev/null

OUTPUT=""

get_bar()
{
	bar=""
	for module in $MODULES; do
		if [[ $INTERNET -eq 0 ]] || [[ $ONLINE_MODULES != *"$module"* ]];then
			module_out="$(sed 's/\.$//g' "$OUTPUT_CACHE$module" 2>/dev/null)"
			bar="$bar$module_out"
		fi
	done
	# Uncomment to remove last separator
	# bar="$(echo "$bar" | sed 's/.$//g')"
	if [[ ${#bar} -ge 2 ]]; then
		bar="${bar::-2}"
	fi
	echo "$LEFT_PADDING$bar$RIGHT_PADDING"
}

# A failing, hanging or missing module only hides itself, never the whole bar
run_module()
{
	local cmd out
	if [[ -x "$CUSTOM_DIR$1" ]]; then
		cmd="$CUSTOM_DIR$1"
	else
		cmd="$DEFAULT_MODULES_DIR$1"
	fi

	if [[ ! -x "$cmd" ]]; then
		: > "$OUTPUT_CACHE$1"
		return
	fi

	out="$(timeout 2 "$cmd" 2>/dev/null)"
	if [[ $? -ne 0 ]]; then
		: > "$OUTPUT_CACHE$1"
	elif [[ "$out" = " " ]]; then
		: > "$OUTPUT_CACHE$1"
	elif [[ -n "$out" ]]; then
		echo "$out$SEPARATOR." > "$OUTPUT_CACHE$1"
	fi
}

run()
{
	for module in $MODULES; do
		[[ ! -f "$OUTPUT_CACHE$module" ]] && touch "$OUTPUT_CACHE$module"
		pgrep -x "$module" &> /dev/null
		notrunning=$([[ $? -eq 1 ]])
		if $notrunning && [[ $INTERNET -eq 0 ]]; then
			run_module $module
		elif $notrunning && [[ $INTERNET -eq 1 ]]; then
			[[ "$ONLINE_MODULES" != *"$module"* ]] && run_module $module
		fi
	done

	get_bar
	sleep $DELAY;
}

run
