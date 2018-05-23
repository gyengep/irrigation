#pragma once
#include <list>
#include <mutex>
#include "IdType.h"

class Program;


class LockedProgram {
	Program* program;
	std::mutex* mutex;

	// Disable copy constructor and operator
	LockedProgram(const LockedProgram&);
	LockedProgram& operator= (const LockedProgram&);

public:
	LockedProgram(Program* program, std::mutex* mutex);
	~LockedProgram();

	Program* operator-> ();
	const Program* operator-> () const;

	const Program* get() const;
	Program* get();
};

typedef std::shared_ptr<LockedProgram> LockedProgramPtr;


///////////////////////////////////////////////////////////////////////////////


class ProgramContainer {
public:
	typedef IdType													key_type;
	typedef Program*												mapped_type;
	typedef std::pair<const key_type, mapped_type>					value_type;
	typedef std::function<bool(const IdType&, LockedProgramPtr)> 	callback_type;

private:

	struct InnerValueType {
		const IdType id;
		Program* program;
		std::mutex* mutex;

		InnerValueType(const IdType id, Program* program);
		~InnerValueType();
	};

	typedef std::list<InnerValueType*> 								container_type;

	// Disable copy constructor and operator
	ProgramContainer(const ProgramContainer&);
	ProgramContainer& operator= (const ProgramContainer&);

	container_type::iterator find(const key_type& key);
	container_type::const_iterator find(const key_type& key) const;

	mutable std::mutex mutex;
	container_type container;

public:
	ProgramContainer();
	virtual ~ProgramContainer();

	virtual void insert(const key_type& key, const mapped_type& value);
	virtual void erase(const key_type& key);
	void iterate(callback_type f) const;

	size_t size() const;
	const LockedProgramPtr at(const key_type& key) const;
	LockedProgramPtr at(const key_type& key);
};

