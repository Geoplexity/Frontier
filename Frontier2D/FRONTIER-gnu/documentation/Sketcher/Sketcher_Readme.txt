SKETCHER README
---------------

	Sketcher is the user interface of the "geometric constraints solver". Sketcher is designed to allow the user to draw sketches on the screen using primitive 
objects line points, lines, rays, line segments, circles and arcs; and then specify the constraints like distance, angle, parallelism, perpendicularity, 
incidence, and tangency between the objects. The sketcher then interprets the sketch drawn by the user and sends the data to the UTU which then further 
carries on the processes of solving the constraints with the help of the DR-Planner, the ESM and the solver. The final solved solution is then displayed 
by the sketcher to the user. 


Drawing a sketch :
	Use the buttons on the main sketcher window to draw the different shapes on the screen. Choose the shape to draw, by clicking in the button
and then by clicking any where on the screen where to draw the object. Similarly to specify constraints between any two objects select one object and then
select the second object while holding the control button down. Selecting two objects can also be done using the selection rectangle. After selecting the two 
shapes to constraint click on the constraint to impose between them. Sketcher allows only the legal constraints to be imposed. for e.g. if two points are 
selected then only distance and incedence constraints are allowed.
	The points can be moved around by simply dragging them with the mouse. But in case of the other objects like the line, circle and the arc they can be 
moved only by moving the points in them one by one.
	The constraints when drawn have their default values namely, distance = -1 and angle = 60 degree. These values can be edited using the panel on the 
right hand side of the screen. First select the constraint. The panel on the right hand side will display the various properties of the constraints like the 
Id's of the shapes involved in the constraints, the constraint value and in case of certain constraints other details for e.g. incase of a distance 
constraint between two objects the panel will display which of the two end points of each of the two lines is involved in the constraint. Now click on the 
field that displays the value of the constraint and edit it.


Grouping of objects :
	Sketcher allows the user to pre-define some objects to be a group, so that when the DR-Planner forms the tree to solve the constraints the user 
defined groups are maintained. Also there is facility to allow two different users to specify diferent groupings in the same sketch. First click on the 
design button on the tool bar and click make new tree. Sketcher then starts to form a new tree with the specified groups. Then select the shapes to be 
grouped together and the click on the make new group button under the design menu. The tree will be displayed on the right hand side panel in the directory 
structure form. It can also be viewed in the tree format on a seperate window by clicking on the group tree button in the design menu. 
	The user can also tell the UTU not to solve a particular group by selecting the group and clicking on the fix button in the design menu.

Simple solver :
	Sketcher also has a inbuilt simple solver which is capable of solving simple constraints as soon as they are input. This does not involve the UTU,
 the DR-Planner or the ESM. This feature can be enabled or disabled. This option is provided in the preferences menu.

Preferences :
	This menu allows the user to pick their own color options for the various shapes and constraints.
	This menu lets the user to choose whether or not to display the groups.
	This menu gives the option to operate in online mode.
	This menu allows the user to enable or disable the simple solver.

Solving a sketch :
	To solve the sketch click the solve button and then in the solve menu there is a option to either solve the sketch in the "autosolve" mode or in the 
"get bifurcation" mode. In the autosolve mode the solver does the job of choosing all the intermediete bifurcations(if any) in the sketch. In the get 
bifurcation mode the user is asked to choose at each step which bifurcation he wants to pick.  If there is any bifurcation that the solver wants the user to 
pick then a bifurcation window will pop up and display only the shapes that have the bifurcation. Then the previous and the next buttons can be used to view
all the possible bifurcations and the select bifurcation  button can be used to pick one of them. At each stage (in the get bifurcation mode) the DR-Tree is 
displayed in a seperate window which displays all the clusters in the tree. The leaves of this tree are the shapes that are be solved. All the clusters that 
have been solved at that point of time will be displayed in black and those that are yet to be solved will be displayed in blue. The cluster that is being 
solved at that time wil be displayed in magenta. This will allow the user to step through the actual process of solving. As and when the sketch is being 
solved the partially solved sketch will be displayed on the main screen (till a point when it can no longer be displayed unless the whole sketch is solved).


Modes of Operation :

1.	Generate mode:- In this mode the user is expected to complete the whole sketch and then solve it. So the sketcher assumes that the sketch is 
complete. By default the sketcher is in the generate mode.

2.	Update mode:- Once the data is sent to the UTU and the DR-Planner sends back a DR-DAG to the sketcher the mode changes to update mode. This means
that the user can stop the solving process at any point by clicking the cancel button on the bifurcation window. Now the sketch can be updated and solved
again. Once in the update mode the options button on the tool bar becomes active. The options menu allows the user to add a new constraint, delete an 
existing constraint, change the value of an existing constraint, add a new shape and constrain it or add a new tree to the sketch. To do any of the above 
click on the appropriate option and then do the changes intended. Then once again click on the autosolve or the get bifurcations button. The option of doing
an update in between the solving process will be available only while solving in the get bifurcations mode.

3.	Online mode:- Another addtional feature of the sketcher is the online mode. Which solves the sketch as and when the constraints are input. The data 
is sent to the DR-Planner and the DR-Dag is obtained. If the tree only contains singleton clusters then the solver is not called. But once a rigid cluster 
is formed the the data is sent to the solver and the solved sketch is displayed. This mode abstracts the user totally from the solving process. But if the
user chooses to step through the solving process then the "online mode with bifurcations" can be used.

Other Features :
1.	Repository:- The repository stores the frequently used sketches. The user can save the most frequently used sketches in the repository and pick up 
any number of instances of those sketches from there whenever required. To use the repository click on the "create new object library" button in the 
repository menu. Select the sketch to be added to the repository and then click on "add selected shapes to library" button in the user objects menu of the 
repository window. Now to import the sketch select the sketch to import and then click on "import the selected user object" button from the user objects menu
then click on the screen where the sketch should be drawn.

2.	Image:- The image shape is used to represent an object (in any representation language) having two handles that are represented using two points on
the corners of the image. This object can also be constrainted just like any other object.