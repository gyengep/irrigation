#!/bin/bash

IS_ENGINEERING_RELEASE=TRUE

PROGRAM_MAJOR=0
PROGRAM_MINOR=2

GIT_DATE=$(git log -1 --pretty=format:'%ai' | cut -c3-4,6-7,9-10)
GIT_REVISION=$(git rev-parse --short HEAD || echo unknown)
GIT_BRANCH=$(git show-ref | grep $(git rev-parse HEAD) | grep -v HEAD | awk '{print $2}' | sed 's|refs/remotes/origin/||' | sed 's|refs/heads/||' | sort | head -n 1)

VERSION="v"
VERSION+="$PROGRAM_MAJOR.$PROGRAM_MINOR"

if [ "$IS_ENGINEERING_RELEASE" = "TRUE" ]; then
	VERSION+="-$GIT_DATE"
	
	if [ -n "$GIT_BRANCH" ]; then
		if [ "$GIT_BRANCH" != "master" ]; then
			VERSION+="-$GIT_BRANCH"
		fi
	else
		VERSION+="<detached>"
	fi
	
	VERSION+="-$GIT_REVISION"
fi

echo "$VERSION"