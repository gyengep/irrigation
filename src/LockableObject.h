/*
 * LockableObject.h
 *
 *  Created on: 30 Jun 2016
 *      Author: pgyenge
 */

#ifndef LOCKABLEOBJECT_H_
#define LOCKABLEOBJECT_H_

template<class TYPE>
class LockableObject {
public:
	mutable std::mutex mutex;
	TYPE object;
};

#endif /* LOCKABLEOBJECT_H_ */
