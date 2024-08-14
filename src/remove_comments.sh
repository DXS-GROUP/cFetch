#!/bin/bash

process_file() {
    local file="$1"
    local temp_file=$(mktemp)

    sed '/^[ \t]*\/\//d' "$file" > "$temp_file"

    mv "$temp_file" "$file"
    echo "Comments removed from $file."
}

process_directory() {
    local dir="$1"
    cd "$dir"

    for entry in *; do
        if [ -d "$entry" ]; then
            if [ "$entry" != "." ] && [ "$entry" != ".." ]; then
                process_directory "$entry"
            fi
        elif [ -f "$entry" ]; then
            if [[ "$entry" == *.c || "$entry" == *.h ]]; then
                process_file "$entry"
            fi
        fi
    done

    cd ..
}

process_directory "."
