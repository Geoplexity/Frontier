/**
 * Title:        SKBifurFrame
 * Description:  Frame used to display bifurcations
 */
   package sketcher;

   import javax.swing.*;
   import javax.swing.border.*;
   import java.awt.*;
   import java.awt.event.*;
   import java.awt.Graphics;

   public class TreeDialog extends JDialog
   {
      public static SKMainFrame frame;
      public static SKGTNArray Groups = new SKGTNArray(100);
      public boolean approved = true;
      public static int[] flag;
      public static SKText mouseOverItem = null;
      public JButton btnContinue = new JButton();
      public JButton btnCancel = new JButton();
      public JPanel pnlButtons = new JPanel();
      public SKScrollableTree pnlMain = null;
      public static int ID;
      public  JStatusBar sbStatus = new JStatusBar();
      public  JToolBar toolbarStatus = new JToolBar();

      public BorderLayout borderLayout1 = new BorderLayout();
      public BorderLayout borderLayout2 = new BorderLayout();
      public  Border border1;
   
      public TreeDialog(Frame owner, SKGTNArray allGroups, int actID, int[] fin, boolean modal)
      {
         super(owner,modal);
         frame = (SKMainFrame)owner;
      
         allGroups.copyArrayTo(Groups);
         setGroups();
         ID = actID;
      //System.out.println("111 sizeof groups = "+Groups.size());
      	
         pnlMain = new SKScrollableTree(this,frame,Groups,null, actID, fin,1);
         flag = new int[fin.length];
         for(int i=0; i<fin.length; i++)
            flag[i] = fin[i];
         pack();
         try
         {
            jbInit();
            pnlMain.drawTree();

         //SKText text = new SKText(pnlMain, this, "test", 100,100);
            setSize(500,350);

         }
            catch(Exception e)
            {
               e.printStackTrace();
            }
      
      }
   
   
      private void jbInit() throws Exception
      {
         border1 = BorderFactory.createBevelBorder(BevelBorder.LOWERED,Color.white,Color.white,new Color(142, 142, 142),new Color(99, 99, 99));
         btnContinue.setText("Continue");
         btnContinue.addActionListener(
                              
                                 new java.awt.event.ActionListener()
                                 
                                 {
                                 
                                 
                                    public void actionPerformed(ActionEvent e)
                                    
                                    {
                                       btnContinue_actionPerformed(e);
                                    }
                                 });
      
         btnCancel.setText("Cancel");
         btnCancel.addActionListener(
                              
                                 new java.awt.event.ActionListener()
                                 
                                 {
                                 
                                 
                                    public void actionPerformed(ActionEvent e)
                                    
                                    {
                                       btnCancel_actionPerformed(e);
                                    }
                                 });
         sbStatus.setPreferredSize(new Dimension(20, 15));
         pnlButtons.setLayout(borderLayout2);
         toolbarStatus.setBorder(border1);
         toolbarStatus.setFloatable(false);
         toolbarStatus.add(sbStatus);
         sbStatus.addPanel("",500,sbStatus.ALIGN_CENTER); //Shape name that mouse it over
         this.addWindowListener(
                                 new java.awt.event.WindowAdapter()
                                 {
                                 
                                    public void windowOpened(WindowEvent e)
                                    {
                                       this_windowOpened(e);
                                    }
                                 });
      
      /*	 pnlMain.addMouseListener(
                                 new java.awt.event.MouseAdapter()
                                 {
      
                                    public void mouseClicked(MouseEvent e)
                                    {
      			System.out.println("111 clicked on pnlMain");
      				
      		if (e.isControlDown())
                                  { //Draw the original tree
                                        drawOriginalTree();
                                  }
                                  else
                                  {   
                                     if(e.getComponent() != null)
                                     {
      			System.out.println(e.getComponent());
                                        mouseOverItem =(SKText)e.getComponent();
                                        reDrawTree();
                                        repaint();
                                     }
                                  }
      
      			
                                    }
       
                                    public void mouseReleased(MouseEvent e)
                                    {
                                    }
                                 });*/
         pnlButtons.add(btnContinue, BorderLayout.WEST);
         pnlButtons.add(toolbarStatus, BorderLayout.SOUTH);
      
         pnlButtons.add(btnCancel, BorderLayout.EAST);
      
         pnlMain.setLayout(borderLayout1);
         pnlMain.setBorder(BorderFactory.createLoweredBevelBorder());
         this.getContentPane().add(pnlMain, BorderLayout.CENTER);
      //         pnlMain.add(pnlShapes, BorderLayout.CENTER);
      //         pnlButtons.add(pnlBifurButtons, BorderLayout.CENTER);
         this.getContentPane().add(pnlButtons, BorderLayout.SOUTH);
      }
      void mouseMoved(MouseEvent e)
      
      {
      //Update mouse coordinates on statusbar
      //sbStatus.updatePanelText(e.getX()+" , "+e.getY(),2);
      
         SKText mouseOn = null;
         mouseOn = (SKText)pnlMain.doHitTest(e.getX(),e.getY());
         if (mouseOn!=null)
         {
            if(!mouseOn.matchingIPName.equals(""))
            {sbStatus.updatePanelText(mouseOn.matchingIPName,0);
               this.repaint();}
         }
      
      }
      public void btnContinue_actionPerformed(ActionEvent e)
      {
         frame.IntArray[0]=frame.mode;
         frame.treeCont = true;
	 System.out.println("treedia "+frame.mode);
         hide();
         frame.RefreshShapeArea();
      }
      public void btnCancel_actionPerformed(ActionEvent e)
      {
         approved = false;
         hide();
         frame.allshapes.copyArrayTo(frame.oldShapes);
         frame.allConstraints.copyArrayTo(frame.oldConstraints);
         frame.oldShapeID = frame.IDCnt - 1;
         frame.oldConstraintID = frame.ConstrIDCnt;
         frame.oldGroupID = frame.GroupIDCnt;
      
         for(int i=0; i<frame.oldConstraints.size(); i++)
         {
            if(frame.oldConstraints.get(i) instanceof SKDistanceConstraint)
               frame.consArray[frame.oldConstraints.get(i).ID] = ((SKDistanceConstraint)frame.oldConstraints.get(i)).distance;
            
            else if(frame.oldConstraints.get(i) instanceof SKAngleConstraint)
               frame.consArray[frame.oldConstraints.get(i).ID] = ((SKAngleConstraint)frame.oldConstraints.get(i)).angle;
         }
      
      }
      public void this_windowOpened(WindowEvent e)
      {this.show();
	    String overConstClusters = "";
            for(int l=0; l<Groups.size(); l++)
                if(Groups.get(l).overConst>0)
                  overConstClusters= overConstClusters.concat(Groups.get(l).name+" ");

            if(!overConstClusters.equals(""))
                JOptionPane.showMessageDialog(frame,"The system is over-constrained at "+overConstClusters);
      }
      public void setGroups()
      {
         if(frame.groupArray.size()>0)
            for(int i=0;i<Groups.size();i++)
            {
               SKGroupTreeNode node = Groups.get(i);
               for(int j=0;j<frame.groupArray.size();j++)
               {
                  SKGroupTreeNode inputDecomp = frame.groupArray.get(j);
                  boolean Match=true;
                  if(inputDecomp.shapes.size()==0)Match=false;
                  if(inputDecomp.shapes.size()!=node.shapes.size())Match=false;
                  else
                     for(int k=0; k<inputDecomp.shapes.size();k++)
                     {
                        Match = (node.shapes.findByID(inputDecomp.shapes.get(k).ID)!=null);
                        if(!Match)
                           break;
                     }
                  if(Match)
                  {
                     if(Groups.get(i).matchingIP.equals(""))Groups.get(i).matchingIP=inputDecomp.name;
                     else Groups.get(i).matchingIP=Groups.get(i).matchingIP.concat(", "+inputDecomp.name);
                  }
               }
            }
      }
      public void reDrawTree()
      {
      
      	System.out.println(" Coming to redraw Tree...");
	if(mouseOverItem == null) 
	  System.out.println(" MOUSE NULL!!!");

         if(mouseOverItem != null)
         {
            SKText name = mouseOverItem;
            String groupName = name.getString();
            int groupID = -1;
            switch(groupName.charAt(0))
            {
               case 'G':
                  if(groupName.charAt(1)=='r')      
                     break;
                  else groupID = Integer.parseInt(groupName.substring(1));
                  break;
               case 'C': groupID = Integer.parseInt(groupName.substring(1));
                  break;
               default:
                  break;
            }
            if(groupID!=-1)
            {
	    	//System.out.println(" The Group/ Cluster ID is: " + groupID);
               SKGTNArray childArray = new SKGTNArray(2);
	       SKGTNArray finalArray = new SKGTNArray(1); // Added to store the final Array
               SKGroupTreeNode node = Groups.findByID(groupID);
               SKGroupTreeNode root = new SKGroupTreeNode(null, node.ID, node.name);
               node.shapes.copyArrayTo(root.shapes);

            //createSubTree(node, root, childArray);
            	finalArray.add(root);
               for(int j=0; j<node.children.size(); j++)
               {
	         //System.out.println(" Root children.... " + node.children.get(j));
                  SKGroupTreeNode child = new SKGroupTreeNode(root,node.children.get(j).ID,node.children.get(j).name);
                  node.children.get(j).shapes.copyArrayTo(child.shapes);
                  root.children.add(child);
		  if(childArray.indexOf(child) == -1)
		  	childArray.add(child);
		  if(finalArray.indexOf(child) == -1)
		  	finalArray.add(child);
               }
		//stdio.getLine();
	       while(childArray.size() > 0)
	       {
	    	   node = Groups.findByID(childArray.get(0).ID);
		   SKGroupTreeNode node2 = finalArray.findByID(childArray.get(0).ID);
		   SKGroupTreeNode node3 = childArray.get(0);
			//System.out.println("Evaluating..... " + childArray.get(0));
		   for(int j=0; j<node.children.size(); j++)
               		{
				//System.out.println("ChildArrays children : " + node.children.get(j));
                  		SKGroupTreeNode child = new SKGroupTreeNode(root,node.children.get(j).ID,node.children.get(j).name);
                  		node.children.get(j).shapes.copyArrayTo(child.shapes);
				if(child.parents.findByID(node2.ID) == null)
				  {
					child.parents.add(node2);
					//System.out.println("     Parent is: " + node2.ID);
				   }
				if(node2.children.findByID(child.ID) == null)
      				        node2.children.add(child);
		 		if(childArray.findByID(child.ID) == null)
		  			childArray.add(child);
		  		if(finalArray.findByID(child.ID) == null)
				{
		  			finalArray.add(child);
					//System.out.println("Added ::: " + child.ID);
				}
              		}
			//stdio.getLine();
		  childArray.removeNode(node3);
	       }

	       for(int l=0; l<finalArray.size(); l++)
               for(int m=0; m<finalArray.get(l).children.size(); m++)
                  for( int n =0 ; n<finalArray.size(); n++)
                     if(finalArray.get(n)==finalArray.get(l).children.get(m))
                        finalArray.get(n).parents.add(finalArray.get(l));

		int[] newFlag = new int[finalArray.size()+1];
	       for(int i = 0; i<finalArray.size(); i++)
                  {
                     SKGroupTreeNode g = Groups.findByID(finalArray.get(i).ID);
                     newFlag[i+1] = flag[Groups.indexOf(g)];
                  }

	       pnlMain.Tree = finalArray;
	       pnlMain.Root = root;
	       pnlMain.ID = root.ID;
	       pnlMain.flag = newFlag;
	       pnlMain.drawTree();


//                if(root!=null)
//                {
//                   childArray.add(root);
//                   for(int i=0; i<root.children.size(); i++)
//                      childArray.add(root.children.get(i));
//
//                   int[] newFlag = new int[childArray.size()+1];
//                   newFlag[0] = flag[Groups.indexOf(node)];
//                   for(int i = 0; i<childArray.size(); i++)
//                   {
//                      SKGroupTreeNode g = Groups.findByID(childArray.get(i).ID);
//                      newFlag[i+1] = flag[Groups.indexOf(g)];
//                   }
//                   pnlMain.Tree=childArray;
//                   pnlMain.Root=root;
//                   pnlMain.ID=root.ID;
//                   pnlMain.flag=newFlag;
//                   pnlMain.drawTree();
               //}
            }
         }
         mouseOverItem = null;
      }

      public void drawOriginalTree()
      {
         pnlMain.Tree=Groups;
         pnlMain.Root=null;
         pnlMain.ID=ID;
         pnlMain.flag=flag;
         pnlMain.drawTree();
      }
      public void createSubTree(SKGroupTreeNode node, SKGroupTreeNode root, SKGTNArray array)
      {

         for(int j = 0; j<node.children.size(); j++)
         {
            SKGroupTreeNode child = new SKGroupTreeNode(root,node.children.get(j).ID,node.children.get(j).name);
            node.children.get(j).shapes.copyArrayTo(child.shapes);
            root.children.add(child);
            array.add(child);
            createSubTree(node.children.get(j),child,array);
         }
      }
   }
