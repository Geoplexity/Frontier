   // SK scrollable tree in version mod 2 done with perfect tree display with minimal crossings.

   package sketcher;

   import java.awt.*;
   import java.awt.event.*;
   import javax.swing.*;
   import javax.swing.border.*;

   public class SKScrollableTree extends scrollableFrame
   {
      public Container container = null;
      public SKGTNArray Tree = null;
      public SKGroupTreeNode Root = null;
      public int[] flag;
      public int ID;
      int contInd;

      public SKMainFrame temp = null;
      public SKTextArray text = new SKTextArray(2);
      public SKEdgeArray edges = null;
      private Color black = Color.black;
      private Color magenta = Color.magenta;
      private Color red = Color.red;
      private Color blue = Color.blue;

      // added this routine for the case of SKUTU file
     //public SKScrollableTree(SKMainFrame frameMain, SKGTNArray t, SKGroupTreeNode r, int active, int[] fin)
//      public SKScrollableTree (SKMainFrame frameMain)
//       {
//          //super(frameMain);
//          temp = SKUTUFile.frameMain;
//          container = null;
//          Tree = SKUTUFile.Groups;
//          Root = null;
//          ID = SKUTUFile.ID;
//          flag = SKUTUFile.flag;
// 
// 
//          try
//          {
//             jbInit();
//             setSize(500,350);
//             /*Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
//             Dimension d = getSize();
//             int x = (screen.width - d.width) >> 1;
//             int y = (screen.height - d.height) >> 1;
//             setLocation(x, y);*/
//          }
//             catch(Exception e)
//             {
//                e.printStackTrace();
//             }
// 
//       }

      //public SKScrollableTree(Container cont,SKMainFrame frameMain, SKGTNArray t, SKGroupTreeNode r, int active, int[] fin)
      public SKScrollableTree(Container cont, SKMainFrame frameMain, SKGTNArray t, SKGroupTreeNode r, int active, int[] fin, int flagCont)
      {
         super(frameMain);
         temp = frameMain;
         container = cont;
         Tree = t;
         Root = r;
         ID = active;
         flag = fin;
         contInd = flagCont;

         try
         {
	
            jbInit();
            setSize(500,350);
            /*Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
            Dimension d = getSize();
            int x = (screen.width - d.width) >> 1;   
            int y = (screen.height - d.height) >> 1;
            setLocation(x, y);*/
         }
            catch(Exception e)
            {      
               e.printStackTrace();
            }
      
      }
      public void jbInit()
      {
         addMouseListener(
                            new java.awt.event.MouseAdapter()
                            {

                               public void mouseClicked(MouseEvent e)
                               {
                                  if (e.isControlDown())
                                  { //Draw the original tree  changed to add container type SKBifurframe2d
				     if(contInd == 0)
				     	SKUTUFile.drawOriginalTree();
			             else
				     if(container instanceof SKBifurFrame2d)
				     	((SKBifurFrame2d)container).drawOriginalTree();
			             else
                                     if(container instanceof TreeDialog)
                                        ((TreeDialog)container).drawOriginalTree();
                                     else
                                        ((TreeFrame)container).drawOriginalTree();
                                     repaint();
                                  }
                                  else
                                  {
                                     if(e.getComponent() != null && doHitTest(e.getX(),e.getY()) != null )
                                     {
				      // Added for SK UTUFIle
				        if(contInd == 0)
					{
					   SKUTUFile.mouseOverItem = (SKText)doHitTest(e.getX(), e.getY());
					   SKUTUFile.reDrawTree();
					}
					else
					if(container instanceof SKBifurFrame2d)
					{
					   ((SKBifurFrame2d)container).mouseOverItem = (SKText)doHitTest(e.getX(),e.getY());
					   if( ((SKBifurFrame2d)container).mouseOverItem == null)
					       System.out.println("NULLLLLL!!!!!!");
					   ((SKBifurFrame2d)container).reDrawTree();
					}
					else
                                        if(container instanceof TreeDialog)
                                        {
                                           ((TreeDialog)container).mouseOverItem =(SKText)doHitTest(e.getX(),e.getY());
                                           ((TreeDialog)container).reDrawTree();
                                        }
                                        else
                                        {
                                           ((TreeFrame)container).mouseOverItem =(SKText)doHitTest(e.getX(),e.getY());
                                           ((TreeFrame)container).reDrawTree();
                                        }
                                        repaint();
                                     }
				     else
				        System.out.println("nulluuuu!!!!");
                                  } 
                               
                               
                               }
                            
                               public void mouseReleased(MouseEvent e)
                               {
                               }
                            
                            });
         addMouseMotionListener(
                              
                                 new java.awt.event.MouseMotionAdapter()
                                 
                                 {

                                    public void mouseMoved(MouseEvent e)
                                    
                                    {
                                       if(container instanceof TreeDialog)
                                          ((TreeDialog)container).mouseMoved(e);
                                    }
                                 });
      
      }
      public void paint(Graphics g)
      {
         super.paint(g);
         for(int i = 0; i<getComponentCount() ;i++)
            ((SKText)getComponent(i)).paintComponent(g);
         g.setColor(black);
         for(int i =0; i<edges.size(); i++)
            g.drawLine(edges.get(i).x1,edges.get(i).y1,edges.get(i).x2,edges.get(i).y2);
      }

      
      // Algorithm for the Draw Tree Comments Included.....

      public void drawTree()
      {
         SKGroupTreeNode temp1; //New addition for display purposed...
         SKGTNArray tree = Tree;
         SKGroupTreeNode root = Root;

         SKGTNArray tempArray = new SKGTNArray(1);
         SKGTNArray finalArray = new SKGTNArray(1);
         SKGTNArray rootArray = new SKGTNArray(1);
         SKGTNArray children = new SKGTNArray(1);
	 SKGTNArray parents = new SKGTNArray(1); // Added array to store the parents...
         SKShapeArray exshapes = new SKShapeArray(1);
	 SKShapeArray drawnShapes = new SKShapeArray(1); // For shapes associated with the childen at lowermost level addition
	 SKGTNArray drawnGroups =  new SKGTNArray(1); //added to see which clusters have been drawn
	 SKGTNArray tempCluster = new SKGTNArray(1); // Array used to store the clusters.
	 SKGTNArray TEMP = new SKGTNArray(1);
	 SKGTNArray TEMP1 = new SKGTNArray(1); // This array needed to do the grouping of the other children.

         edges = new SKEdgeArray(2);

         int center = 0;
         int x1,y1,x2,y2,total=0;
	 int currLevel, maxLevel = 0; //Var to calculate the max level.
	 int currY, currXoffset;


         for(int i = temp.SelectedShapes.size() - 1 ; i >= 0 ;i--)
            temp.removeSelectedShape(temp.SelectedShapes.get(i));



         text.clear();
         this.removeAll();
         tree.copyArrayTo(tempArray);


	// First calculate the levels for all thanodes of the Tree... we are building this tree bottom up!
         for(int i=0; i< tempArray.size();i++)
         {
	 	SKGroups.setLevels(tempArray);
		temp1 = tempArray.get(i);
		if(temp1.level > maxLevel) maxLevel = temp1.level; // Find the max level
         }
	 maxLevel++;
	 //stdio.getLine();

	 currLevel = maxLevel;

	 currY = 25 + 50 * currLevel;
	 currXoffset = 20;
	 int totShapes = 0;

	 for(int i = 0; i < tempArray.size(); i++)
	 {
	 	if(tempArray.get(i).children.size() == 0)
			{

				totShapes += tempArray.get(i).shapes.size();
				if(tempArray.get(i).level == maxLevel - 1)
				{
			 	    children.add(tempArray.get(i));
				}

			}

	}

	int distBetweenShapes = this.getWidth();
	if(totShapes > 0)
		 distBetweenShapes = this.getWidth() / totShapes;

	// Calculate the total shapes at the bottomost level and accordingly find the distance between the shapes....
	parents.clear();
	SKGroupTreeNode child;
	SKGroupTreeNode parent;
	child = children.get(0);
	tempCluster.add(child);
	children.removeNode(child);
	int l = 0;

	
	// Run basic clustering technique to cluster the nodes present at that level.... (It is very primitive method just groups the clusters according to the common parents..)
	while(children.size() > 0)
	{
	while(l < tempCluster.size())
	{
		SKGroupTreeNode t = tempCluster.get(l);
		for(int j = 0; j < t.parents.size(); j++)
		{
			if(parents.indexOf(t.parents.get(j)) == -1)
			{
				for(int k = 0;k < children.size(); k++)
				{

					if(children.get(k).parents.indexOf(t.parents.get(j)) != -1)
					{
						child = children.get(k);
						tempCluster.add(child);
						parents.add(t.parents.get(j));
					}
				}
				for(int k = 0; k< tempCluster.size(); k++)
				{
					if(children.indexOf(tempCluster.get(k)) != -1)
						children.removeNode(tempCluster.get(k));
				}
			}
		}
		l++;
	}
	if(children.size() == 0) break;
	child = children.get(0);
	tempCluster.add(child);
	children.removeNode(child);
	}


	children.clear();
	tempCluster.copyArrayTo(children);
	// Arrange all the shapes nodes at the bottomost level and fix the x,y coordinates for their parents at that level Remember to remove the printf's.

	int sx, sy;
	int[] xArray;
	xArray = new int[totShapes + 10];
	for(int i=0; i<children.size(); i++)
	{
		sy = currY;
		if(children.get(i).level == maxLevel - 1)
		{ //System.out.println(children.get(i).name + " Shape" + children.get(i).shapes.size());
			for(int j =0; j< children.get(i).shapes.size(); j++)
			{
				//if(children.get(i).shapes.get(j) == null)
				// 		System.out.println(" Child: " + children.get(i).name + " " + j +" " + children.get(i).shapes.size());

				if( drawnShapes.indexOf(children.get(i).shapes.get(j)) == -1 || children.get(i).shapes.get(j) == null)
				{
					sx = currXoffset + drawnShapes.size() * distBetweenShapes;
					xArray[drawnShapes.size()] = sx;
					//if(children.get(i).shapes.get(j) != null)
					   drawnShapes.add(children.get(i).shapes.get(j));

                  			SKText shapeName = null;
                  			if(children.get(i).shapes.get(j)!=null)
                    			{
                     					shapeName = new SKText(this,container,children.get(i).shapes.get(j).Name.charAt(0)+""+children.get(i).shapes.get(j).ID,sx,sy);
                     					temp.addSelectedShape(children.get(i).shapes.get(j));

                    			}
                  			else
					{
						//System.out.println("x,y" +  " " + children.get(i).name + " " + children.get(i).y + " " +finalArray.size());
                     				shapeName = new SKText(this,container,"U",sx,sy);
					}
                  				shapeName.setColor(red);

                  			if(text.findByValue(shapeName.value) == null)
                     			text.add(shapeName);

					if(finalArray.indexOf(children.get(i)) == -1)
					{
						if(children.get(i).shapes.size() == 1)
						{
							children.get(i).x = sx;
						}
						else
						{
							children.get(i).x = sx + (distBetweenShapes / 2);
						}
						children.get(i).y = currY - 50;
						children.get(i).width = distBetweenShapes;
						finalArray.add(children.get(i));
						//System.out.println("Added in " + children.get(i).ID + " "  + finalArray.size());
					}
					//else
					    //System.out.println("ID " + " "+ children.get(i).ID);
					SKEdge edge = new SKEdge(children.get(i).x,children.get(i).y,sx,sy);
                  		        edges.add(edge);
				}
				else
				{
					SKEdge edge = new SKEdge(children.get(i).x,children.get(i).y,xArray[drawnShapes.indexOf(children.get(i).shapes.get(j))], sy);
					//System.out.println("Y1 : " + children.get(i).y + " " + children.get(i).ID + " " + children.get(i).shapes.get(j).ID);
                  			edges.add(edge);
				}
				//System.out.println(children.get(i).name + " in main level " +  children.get(i).y);

			}
		}
	}
	//stdio.getLine();
	currLevel--; currY -=50;
	int distClusters ;
	//stdio.getLine();

	// For noodes at each level excluding the bottomost level.... that is the level with only shapes the following loop
	// calculates the arrangement of the tree as.....
	for(int i = currLevel ; i>0;i--)
	{

		drawnGroups.clear();;
		TEMP.clear();
		TEMP1.clear();
		tempCluster.clear();
		for(int j = 0; j< children.size(); j++)
		{
			TEMP.add(children.get(j));
			tempCluster.add(children.get(j));
		}

		for(int j = 0;j<tempArray.size();j++) // Add the children i.e leaf nodes to a temp Array
		{
			if(tempArray.get(j).children.size() == 0 && tempArray.get(j).level == currLevel)
				TEMP1.add(tempArray.get(j));
		}

		parents.clear();
		// First of all it finds the nodes at that particular leval and tries to cluster them
		if(TEMP1.size() > 0)
		{
			l = 0;
			while(TEMP1.size() > 0)
			{
				while(l < tempCluster.size())
				{
					SKGroupTreeNode t = tempCluster.get(l);
					for(int j = 0; j < t.parents.size(); j++)
					{
						if(parents.indexOf(t.parents.get(j)) == -1)
						{
							for(int k = 0;k < TEMP1.size(); k++)
							{
								if(TEMP1.get(k).parents.indexOf(t.parents.get(j)) != -1)
								{
									child = TEMP1.get(k);
									tempCluster.add(child);
									parents.add(t.parents.get(j));

								}
							}
							for(int k = 0; k< tempCluster.size(); k++)
							{
								if(TEMP1.indexOf(tempCluster.get(k)) != -1)
									TEMP1.removeNode(tempCluster.get(k));
							}
						}
					}
					l++;
				}
				if(TEMP1.size() == 0) break;
				child = TEMP1.get(0);
				tempCluster.add(child);
				TEMP1.removeNode(child);
			}

			for(int j = 0; j< tempCluster.size();j++)
			{
				TEMP.add(tempCluster.get(j));
			}
		}
		int numClusters = 0;
		// Find the positions for the shape nodes at that level... the "null " condition checks are made for the 'U' nodes in the
		// 3D mode and are helpful as the 'U' nodes have null shapes as their children.

		TEMP.copyArrayTo(children);
		for(int  j = 0; j< children.size(); j++)
		{
			if(children.get(j).children.size() == 0)
			{
				for(int k = 0;k<children.get(j).shapes.size(); k++)
				{
					sy = currY + 50;
					if(drawnShapes.indexOf(children.get(j).shapes.get(k)) == -1 || (children.get(j).shapes.get(k) == null && finalArray.indexOf(children.get(j)) == -1))
					{
						SKBaseShape temp2 = children.get(j).shapes.get(k);

						sx = currXoffset + drawnShapes.size() * distBetweenShapes;
						xArray[drawnShapes.size()] = sx;
						//if(children.get(j).shapes.get(k) != null)
							drawnShapes.add(children.get(j).shapes.get(k));

						SKText shapeName = null;
                  				if(children.get(j).shapes.get(k)!=null)
                    				{
                     					shapeName = new SKText(this,container,children.get(j).shapes.get(k).Name.charAt(0)+""+children.get(j).shapes.get(k).ID,sx,sy);

                     					temp.addSelectedShape(children.get(j).shapes.get(k));

                    				}
                  				else
						{
						//System.out.println("x,y down" + children.get(j).name + " " + children.get(j).x + " " + children.get(j).y);
                     					shapeName = new SKText(this,container,"U",sx,sy);
							}
                  				shapeName.setColor(red);

                  				if(text.findByValue(shapeName.value) == null)
                     				text.add(shapeName);

						if(drawnGroups.indexOf(children.get(j)) == -1)
						{
						if(children.get(j).shapes.size() == 1)
						{
							children.get(j).x = sx;
						}
						else
						{
							children.get(j).x = sx + (distBetweenShapes / 2);
						}
						children.get(j).y = currY;
						children.get(j).width = distBetweenShapes;
						finalArray.add(children.get(j));
					        }


						SKEdge edge = new SKEdge(children.get(j).x, children.get(j).y,sx,sy);
                  		        	edges.add(edge);

					}
					else
					{
						if(children.get(j).shapes.get(k) != null)
						{
							SKEdge edge = new SKEdge(children.get(j).x,children.get(j).y,xArray[drawnShapes.indexOf(children.get(j).shapes.get(k))], sy);
							//System.out.println("Y2 : " + children.get(j).y + " "+ children.get(j).ID );
                  					edges.add(edge);
						}

					}
					//System.out.println( children.get(j).name + " " + children.get(j).y);
				}
			}
			//stdio.getLine();
		}
		parents.clear();
		drawnGroups.clear();




		// Loop to calculate the parents number...... added here this is to calculate the distance between the parents.
		for(int j = 0; j <tempArray.size(); j++)
		{
			if(tempArray.get(j).level == currLevel - 1) // Level of parents
 			{

				if(tempArray.get(j).children.size() != 0)
						numClusters++;

			}

		}


 		distClusters = (int) (this.getWidth() - ((totShapes - drawnShapes.size()) * distBetweenShapes)) / (numClusters + 1);
 		drawnGroups.clear();
		parents.clear();

		// arranfe the nodes at that level  accordingly fixing up their positions as well as their parents position.
		for(int j = 0; j< children.size(); j++)
		{

			for(int k = 0; k < children.get(j).parents.size(); k++)
			{
				if( (finalArray.indexOf(children.get(j).parents.get(k)) == -1) && (children.get(j).parents.get(k).level == i - 1)) // parent not already added and is one level up
				{
					children.get(j).parents.get(k).x = distClusters * (drawnGroups.size() + 1); //children.get(j).x + (distClusters / 2); // + (parents.size() * distClusters);
					children.get(j).parents.get(k).y = currY - 50;
					drawnGroups.add(children.get(j).parents.get(k));
					finalArray.add(children.get(j).parents.get(k));
					parents.add(children.get(j).parents.get(k));
					SKEdge edge = new SKEdge(children.get(j).x,children.get(j).y,children.get(j).parents.get(k).x, children.get(j).parents.get(k).y);
                  		        edges.add(edge);
				}
				else
				{
					if(children.get(j).parents.get(k).level == i - 1)
					{
						SKEdge edge = new SKEdge(children.get(j).x,children.get(j).y,children.get(j).parents.get(k).x, children.get(j).parents.get(k).y);
                  		        	edges.add(edge);
					}
				}

			}
		}

		for(int j=0; j <children.size(); j++)
		{
			for(int k = 0 ; k< children.get(j).children.size(); k++)
			{

				if((children.get(j).children.get(k).level - children.get(j).level ) > 1)
				{
						SKEdge edge = new SKEdge(children.get(j).x,children.get(j).y,children.get(j).children.get(k).x, children.get(j).children.get(k).y);
						//System.out.println("Y3 : " + children.get(j).y + " " + children.get(j).ID + " " + children.get(j).children.get(k).ID);
                  		        	edges.add(edge);
				}
			}
		}
		//stdio.getLine();
		currLevel--;
		currY -= 50;
		children.clear();
		parents.copyArrayTo(children);

	}

// Fix up the names of the clusters to be shown in the tree.

	for(int i=0; i<finalArray.size(); i++)
         {
            SKText groupName = null;
            if(finalArray.get(i).parents.size()==0 && finalArray.get(i).parent==null)
               groupName = new SKText(this,container,finalArray.get(i).name,finalArray.get(i).x,finalArray.get(i).y);
            else if(ID==0)
               groupName = new SKText(this,container,"G"+finalArray.get(i).ID,finalArray.get(i).x,finalArray.get(i).y);
            else
               groupName = new SKText(this,container,"C"+finalArray.get(i).ID,finalArray.get(i).x,finalArray.get(i).y);

            if(groupName!=null && !finalArray.get(i).matchingIP.equals(""))
            {
               groupName.setMatchingIPName(finalArray.get(i).matchingIP);
            }


	//System.out.println("Name:  " + groupName.value + " matchingIPName " + groupName.matchingIPName);
	//stdio.getLine();

            if(finalArray.get(i).ID==ID)
               groupName.setColor(magenta);
            else if (flag[Tree.indexOf(finalArray.get(i))]==1)
               groupName.setColor(blue);
            else groupName.setColor(black);

            if(groupName!=null)
            {
               groupName.repaint();
               if(text.findByValue(groupName.value) == null)
                  text.add(groupName);
            }

         }


      }
      public void addComponents()
      {
         for(int i=0 ;i <text.size(); i++)
            this.add(text.get(i));
      }

      public SKText doHitTest(int x, int y)
      {
         for(int i=0 ; i<text.size(); i++)
         {
            if(x>text.get(i).x-10 && x<text.get(i).x+15)
               if(y>text.get(i).y-10 && y<text.get(i).y+15)
                  return text.get(i);
         }
         return null;
      }
   }
