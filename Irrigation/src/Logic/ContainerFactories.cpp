#include "common.h"
#include "ContainerFactories.h"


template <> std::unique_ptr<Factory<RunTimeContainer>> RunTimeContainerFactoryHolder::factory(new RunTimeContainerFactory());
template <> std::unique_ptr<Factory<StartTimeContainer>> StartTimeContainerFactoryHolder::factory(new StartTimeContainerFactory());
template <> std::unique_ptr<Factory<ProgramContainer>> ProgramContainerFactoryHolder::factory(new ProgramContainerFactory());
