#!/bin/bash

IS_ENGINEERING_RELEASE=TRUE

PROGRAM_MAJOR=0
PROGRAM_MINOR=2
PROGRAM_PATCH=

COMMIT_DATE=$(git log -1 --pretty=format:'%ai' | cut -c3-4,6-7,9-10)
COMMIT_HASH=$(git rev-parse --short=8 HEAD || echo unknown)
GIT_BRANCH=$(git show-ref | grep $(git rev-parse HEAD) | grep -v HEAD | awk '{print $2}' | sed 's|refs/remotes/origin/||' | sed 's|refs/heads/||' | sort | head -n 1)

VERSION="v"
VERSION+="$PROGRAM_MAJOR.$PROGRAM_MINOR"

if [ -n "$PROGRAM_PATCH" ]; then
	VERSION+=".$PROGRAM_PATCH"
fi

if [ "$IS_ENGINEERING_RELEASE" = "TRUE" ]; then
	VERSION+="-$COMMIT_DATE"
	
	if [ -n "$GIT_BRANCH" ]; then
		if [ "$GIT_BRANCH" != "master" ]; then
			VERSION+="-$GIT_BRANCH"
		fi
	else
		VERSION+="-unknown"
	fi
	
	VERSION+="-$COMMIT_HASH"
fi

echo "$VERSION"