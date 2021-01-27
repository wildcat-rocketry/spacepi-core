Project Architecture
====================

Micro-Service Architecture
--------------------------

When an application built on the SpacePi framework is running, it runs as a set of modules which do not share memory, or possibly even hardware resources, with the other processes.
Each hardware driver, calculation script, control system, etc. runs as a different module under this framework.
To communicate, each module can publish messages and subscribe to messages which are published by other modules.
When one module publishes a message, the core library ensures that every other module which subscribes to that message receives it and processes it.
Additionally, the dashboard can be used to publish or subscribe to certain messages allowing debugging of a single module without having to start up and debug the whole system.

Decentralized Architecture
--------------------------

Each host machine running the application is fully capable of running on its own, independently of the other hosts.
However, they can also connect to other hosts and transmit messages from one module on one host to another module on another host.
The core library is designed in a decentralized way, meaning that if a host goes down, no matter which host or how many hosts, the remaining alive hosts will still continue to function normally.

This architecture differs from a normal client-server pattern because with that pattern, since that is a centralized approach.
If each client relies on the server to handle passing messages between the different modules, then if the server goes down, all communication links are lost and the application will cease to function.
However, in this situation, the SpacePi framework would continue routing messages to each module, but just taking a different path in order to avoid the broken host.

This design has many advantages, but the largest one for the application of flight computers is during stage separation.
Since there is no central host, when a rocket splits in half, the different hosts on each part of the rocket will automatically form new communication channels to handle the fact that half of the hosts are on the other stage and are now out of range.
This way, inter-host communication can still happen with the two stages separated, and without any extra configuration.

SpacePi Repositories
--------------------

The SpacePi code is split up between multiple Git repositories.
There are a few main types of repositories in which the code can exist, which are:

1. Core Repository
2. Modules Repository
3. Target Repository
4. Packages Repository
5. Template Repositories

Core Repository
***************

The core repository contains the code documented by this website, and also the majority of the code which makes all of the various pieces work together.
This repository does not need to be edited frequently, as it contains no details about any implementation, platform, etc. in the SpacePi system, but rather just the core concepts and components.

Modules Repository
******************

The modules repository is where all of the services specific to the actual program implementation are stored.
These services may include hardware drivers, logic services (for performing large calculations), control services (using sensor inputs and calculations to make control decisions), etc.
This repository contains the bulk of the application-specific code, and therefore is edited much more frequently.

Target Repository
*****************

The target repository contains the code which handles the platform on which the application is running.
This may contain functionality such as how to boot up a processor, how to connect to hardware resources such as digital IO or busses, etc.
This repository is also not edited much like the core repository, as it does not have any tie to the application-specific code.

Packages Repository
*******************

The packages repository contains configuration files which tie the whole system together.
From these configuration files, targets and modules are linked together to form a working application.
This repository will be edited when changing big-picture application requirements, but is not edited as often as the modules since the packages repository does not actually contain any code.

Template Repository
*******************

The template repositories are intended as a starting point for creating new sets of modules, targets, and/or packages.
When creating a new repository for the SpacePi system, GitHub can use one of these templates as a starting point to make development easier.
