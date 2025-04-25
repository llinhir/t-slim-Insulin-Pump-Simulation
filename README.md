# t-slim-Insulin-Pump-Simulation

Qt C++ simulation
Team 22
Collaborators: Arjun Bedi, Eli Roberts, Katherine Ma, Jessica Hill
Github: https://github.com/llinhir/t-slim-Insulin-Pump-Simulation

What each member did:
(we had a lot of overlap on what we worked on since we worked in-person a lot and did alot of pair programming)
Katherine - Made the simulation UI, coded the login/options page + insulin.cpp + bolus graph ploting
Jessica - Coded the bolus.cpp page + calculations + alerts + logger + state diagram
Eli - coded the profile.cpp and create profile.cpp pages
Arjun - coded machine.cpp + bolus graph initialzation + simulation.cpp + history page + blood glucose vector/data

Documentation:
Use Cases: Arjun, Eli, Jessica, Kathy
Use case diagram: Jessica
Class diagram: Kathy
Sequence diagrams: Eli
State machine: Jessica
Tracability matrix: Jessica

Note: all our documentation (minus class diagram) is in the document
---------------------------------------

Header Files:
defs.h
bolus.h
createprofile.h
insulin.h
machine.h
mainwindow.h
options.h
profile.h
simulation.h

Source Files:
bolus.cpp
createprofile.cpp
insulin.cpp
machine.cpp
main.cpp
mainwindow.cpp
mainwindow.ui
options.cpp
profile.cpp
simulation.cpp

Others
Code.pro
Code.pro.user
.gitignore
...

Note: Qt chart was used so to install it, run the following command in terminal:
sudo apt install libqt5charts5-dev
