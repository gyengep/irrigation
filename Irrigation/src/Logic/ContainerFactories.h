#pragma once

#include "Containers.h"


class RunTimeContainerFactory : public Factory<RunTimeContainer> {
public:
	RunTimeContainer* create() { return new RunTimeContainer(); }
};


class StartTimeContainerFactory : public Factory<StartTimeContainer> {
public:
	StartTimeContainer* create() { return new StartTimeContainer(); }
};


class ProgramContainerFactory : public Factory<ProgramContainer> {
public:
	ProgramContainer* create() { return new ProgramContainer(); }
};


typedef FactoryHolder<RunTimeContainer> RunTimeContainerFactoryHolder;
typedef FactoryHolder<StartTimeContainer> StartTimeContainerFactoryHolder;
typedef FactoryHolder<ProgramContainer> ProgramContainerFactoryHolder;
