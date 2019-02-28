/**
 * Title:        SKBifurFrame
 * Description:  Frame used to display bifurcations
 */
   package sketcher;
   import javax.swing.*;
   import java.awt.*;
   import java.awt.event.*;
   import java.awt.Graphics;
   import java.awt.geom.*;

   import javax.media.j3d.*;
   import javax.vecmath.*;

   public class SKBifurFrame extends JDialog
   {
      public SKMainFrame frameMain;
      public int currBifurcation = -1;
      public int lastHorizScroll = 0,lastVertScroll = 0;
      public int horvalue = 0;
      public int vervalue = 0;
   	public int reSolveID=-1;
      public float              lastScale = 1;
      public boolean approved = true;
      public SKBifurcationArray bifurcations;
      public SKShape3dArray Shapes = new SKShape3dArray(2);
      public SKConstraint3dArray arrCon = null;
      public JButton btnPrevious = new JButton();
      public JButton btnNext = new JButton();
      public JButton btnConfirm = new JButton();
      public JButton btnResolve = new JButton();
      public JButton btnCancel = new JButton();
      public JDialog reSolveDialog =null;
      public JLabel lblStatus = new JLabel();
      public JLabel lblScale = new JLabel();
      public JPanel pnlBifurButtons = new JPanel();
      public JPanel pnlMain = new JPanel();
      public JPanel pnlControls = new JPanel();
      public JPanel pnlButtons = new JPanel();
      public JPanel pnlConfirm = new JPanel();
      public JSelectionPanel pnlShapes = new JSelectionPanel(frameMain);
      public JSelectionPanel3d pnlShapes3d = null;
      public JScrollBar scrVert = new JScrollBar();
      public JScrollBar scrHoriz = new JScrollBar();
      public JComboBox cmbScale = new JComboBox();
      public BorderLayout borderLayout1 = new BorderLayout();
      public BorderLayout borderLayout2 = new BorderLayout();
   
      public SKBifurFrame(Frame owner, String title, boolean modal)
      {
         super(owner,title,modal);
         frameMain = (SKMainFrame)owner;
         pnlShapes = new JSelectionPanel(frameMain);
         pnlShapes3d = new JSelectionPanel3d(frameMain);
         try
         {
            jbInit();
            setSize(600,490);
         //Center on desktop
            Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
            Dimension d = getSize();
            int x = (screen.width - d.width) >> 1;
            int y = (screen.height - d.height) >> 1;
           // setLocation(x, y);
         }
            catch(Exception e)
            {
               e.printStackTrace();
            }
      
      }
   
   
      private void jbInit() throws Exception
      {
         pnlMain.setLayout(borderLayout1);
         scrHoriz.setOrientation(JScrollBar.HORIZONTAL);
         scrHoriz.addAdjustmentListener(
                                 new java.awt.event.AdjustmentListener()
                                 {
                                 
                                    public void adjustmentValueChanged(AdjustmentEvent e)
                                    {
                                       scrHoriz_adjustmentValueChanged(e);
                                    }
                                 });
         scrVert.addAdjustmentListener(
                                 new java.awt.event.AdjustmentListener()
                                 {
                                 
                                    public void adjustmentValueChanged(AdjustmentEvent e)
                                    {
                                       scrVert_adjustmentValueChanged(e);
                                    }
                                 });
         btnPrevious.setText("Previous");
         btnPrevious.addActionListener(
                              
                                 new java.awt.event.ActionListener()
                                 
                                 {
                                 
                                 
                                    public void actionPerformed(ActionEvent e)
                                    
                                    {
                                       btnPrevious_actionPerformed(e);
                                    }
                                 });
         lblStatus.setText(" Bifurcation 1 / N ");
         btnNext.setText("Next");
         btnNext.addActionListener(
                              
                                 new java.awt.event.ActionListener()
                                 
                                 {
                                 
                                 
                                    public void actionPerformed(ActionEvent e)
                                    
                                    {
                                       btnNext_actionPerformed(e);
                                    }
                                 });
      
         pnlShapes.setLayout(null);
         cmbScale.setEditable(true);
         cmbScale.addItemListener(
                                 new java.awt.event.ItemListener()
                                 {
                                 
                                    public void itemStateChanged(ItemEvent e)
                                    {
                                       cmbScale_itemStateChanged(e);
                                    }
                                 });
         lblScale.setText("Scale");
         pnlMain.setBorder(BorderFactory.createLoweredBevelBorder());
         pnlButtons.setLayout(borderLayout2);
         btnConfirm.setText("Select Bifurcations");
         btnConfirm.addActionListener(
                                 new java.awt.event.ActionListener()
                                 {
                                 
                                    public void actionPerformed(ActionEvent e)
                                    {
                                       btnConfirm_actionPerformed(e);
                                    }
                                 });
         btnResolve.setText("Re-Do");
         btnResolve.addActionListener(
                              
                              
                                 new java.awt.event.ActionListener()
                                 
                                 
                                 {
                                 
                                 
                                 
                                    public void actionPerformed(ActionEvent e)
                                    
                                    
                                    {
                                       btnResolve_actionPerformed(e);
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
      
         this.addWindowListener(
                                 new java.awt.event.WindowAdapter()
                                 {
                                 
                                    public void windowOpened(WindowEvent e)
                                    {
                                       this_windowOpened(e);
                                    }
                                 });
         pnlBifurButtons.add(btnPrevious, null);
         pnlBifurButtons.add(lblStatus, null);
         pnlBifurButtons.add(btnNext, null);
         pnlButtons.add(pnlConfirm, BorderLayout.SOUTH);
         pnlConfirm.add(btnConfirm, null);
	 pnlConfirm.add(btnResolve, null);
         pnlConfirm.add(btnCancel, null);
         this.getContentPane().add(pnlMain, BorderLayout.CENTER);
         //pnlMain.add(pnlShapes, BorderLayout.CENTER);
         pnlMain.add(pnlShapes3d, BorderLayout.CENTER);
         pnlMain.add(scrVert, BorderLayout.EAST);
         pnlMain.add(scrHoriz, BorderLayout.SOUTH);
         this.getContentPane().add(pnlControls, BorderLayout.NORTH);
         pnlControls.add(lblScale, null);
         pnlControls.add(cmbScale, null);
         pnlButtons.add(pnlBifurButtons, BorderLayout.CENTER);
         this.getContentPane().add(pnlButtons, BorderLayout.SOUTH);
      
         scrHoriz.setValues(0,pnlShapes.getWidth(),-1000,1000);
         scrHoriz.setUnitIncrement(10);
         scrVert.setValues(0,pnlShapes.getHeight(),-1000,1000);
         scrVert.setUnitIncrement(10);
      
         for(int i=0; i<frameMain.scaleValues.length; i++)
            cmbScale.addItem(frameMain.scaleValues[i]);
         cmbScale.setSelectedIndex(1);
      
      }
   
      void this_windowOpened(WindowEvent e)
      
      {         
         setBifurcation(0);
      
         boolean yes=false; 
         SKBaseShape3d fix = null;
         SKBaseShape3d fix1 = null; 
         int count =0;
         JOptionPane dialog = null;
/*         for(int i=0; i<pnlShapes3d.root_group.numChildren();i++)
         {
//            SKBaseShape3d sh = getShape3d(pnlShapes3d.root_group.getChild(i).getUserData());
	     SKBaseShape3d sh = (SKBaseShape3d)pnlShapes3d.root_group.getChild(i);
            if((sh instanceof SKPointSubShape3d) && (Shapes.indexOf(sh.getSelectable())==-1))
               Shapes.add(sh.getSelectable());
            else if (!(sh instanceof SKPointSubShape3d))
               Shapes.add(sh);
         }
*/
         for(int i=0; i<pnlShapes3d.root_group.numChildren();i++)
         {
            SKBaseShape3d try1 = null;
	    if( getShape3d(pnlShapes3d.root_group.getChild(i).getUserData())!=null)
	    {
	     try1 = getShape3d(pnlShapes3d.root_group.getChild(i).getUserData());
             int id = try1.ID;
             if(!(try1 instanceof SKPointSubShape3d))
             {
               SKBaseShape3d try2 = frameMain.allshapes3d.findByID(id);
               if(try2.fixed==true)
               {
                  yes = true;
                  fix=try1.getSubShape(1);
                  fix1=try2.getSubShape(1);
                  count++;
               }
               if(frameMain.solvingShapes3d.indexOf(try1) == -1)
                  frameMain.solvingShapes3d.add(try1);
             }
	    }
         }
/*	 for(int i=0; i<Shapes.size(); i++)
	  for(int j=0; j<Shapes.get(i).ConstraintList.size(); j++)
	  {
//	     System.out.println("111 list of "+Shapes.get(i)+" list = "+Shapes.get(i).ConstraintList.size());
	     Shapes.get(i).ConstraintList.get(j).doPaint(pnlShapes3d);
	  }*/
         repaint();
         //if(count>1)
            //dialog.showMessageDialog(frameMain.panelShapeArea,"can not display partially solved sketch");
      }
      public void paint(Graphics g)
      {
         super.paint(g);
         for(int i=0; i<Shapes.size(); i++)
            Shapes.get(i).paintConstraints(g);
         //pnlShapes.drawNames(g);
         return;
      }
   /*
      void pnlShapes_mouseMoved(MouseEvent e)
      {
      //Update mouse coordinates on statusbar
      //sbStatus.updatePanelText(e.getX()+" , "+e.getY(),2);
         boolean wasNull = (frameMain.mouseOverItem==null);
         if (frameMain.mouseOverItem==null || !frameMain.mouseOverItem.getClonedItem().doHitTest(e.getX(),e.getY()) )
         {
            for (int i=0; i<frameMain.DrawnItems.size(); i++)
            {
               frameMain.mouseOverItem = frameMain.DrawnItems.get(i);
               if ( frameMain.mouseOverItem.getClonedItem().doHitTest(e.getX(),e.getY()) )
               {
                  frameMain.sbStatus.updatePanelText( frameMain.mouseOverItem.toString(),3);
                  pnlShapes.repaint();
                  return;
               }
            }
         
            frameMain.mouseOverItem=null;
            frameMain.sbStatus.updatePanelText("",3);
            if (!wasNull)  pnlShapes.repaint();
         }
      }
   
      void pnlShapes_mouseDragged(MouseEvent e)
      {
       For dragging DrawnItems (dragging lineShape by line)
      if (mouseOverItem != null && mouseOverItem instanceof SKBaseShape)
      { //Drag shape
      SKBaseShape sh = (SKBaseShape)mouseOverItem;
      if (sh.isSelected())
      {
        if (sh.DragState==-1)
        {
          sh.DragState = 0;
          sh.DragX = e.getX();
          sh.DragY = e.getY();
          sh.setCursor( Cursor.getPredefinedCursor(Cursor.MOVE_CURSOR) );
        }
        else
        { //Draw as user is dragging
          sh.doMove(e.getX()-sh.DragX,e.getY()-sh.DragY,(SKOptions.byteOptions[ SKOptions.simpleSolverMode ]==2));
        }
      
        RefreshShapeArea();
      }
      }
      else
      { //Do selection rect
      
         if (!pnlShapes.DrawSelRect)
         {
            pnlShapes.DrawSelRect = true;
         
            pnlShapes.StartX = e.getX();
            pnlShapes.StartY = e.getY();
         }
         else
         { //Draw selection rect
            pnlShapes.EndX = e.getX();
            pnlShapes.EndY = e.getY();
         
            pnlShapes.repaint();
         }
      //    }
      }
   
      void pnlShapes_mouseReleased(MouseEvent e)
      {
         if (frameMain.mouseOverItem != null && frameMain.mouseOverItem instanceof SKBaseShape)
            ((SKBaseShape)frameMain.mouseOverItem).DragState = -1;
      
         if (e.isPopupTrigger())
         {
            frameMain.popupShape.show(pnlShapes,e.getX(),e.getY());
            return;
         }
      
         if (pnlShapes.DrawSelRect)
         { //Selected shapes in rect and repaint
            if (!e.isControlDown()) frameMain.clearSelectedShapes(true,true);
         
            int sx,sy,ex,ey;
            if (pnlShapes.StartX<pnlShapes.EndX)
            {
               sx = pnlShapes.StartX;
               ex = pnlShapes.EndX;
            }
            else
            {
               sx = pnlShapes.EndX;
               ex = pnlShapes.StartX;
            }
            if (pnlShapes.StartY<pnlShapes.EndY)
            {sy = pnlShapes.StartY;
               ey = pnlShapes.EndY;
            }
            else
            {
               sy = pnlShapes.EndY;
               ey = pnlShapes.StartY;
            }
         
            SKBaseShape sh;
            for (int i=0; i<pnlShapes.getComponentCount(); i++)
            {
               sh = (SKBaseShape)pnlShapes.getComponent(i);
               if (sh.getX()>sx && (sh.getX()+sh.getWidth())<ex &&
                  sh.getY()>sy && (sh.getY()+sh.getHeight()<ey))
               {
                  frameMain.addSelectedShape(sh);
               }
            }
         
            pnlShapes.DrawSelRect=false;
            pnlShapes.repaint();
         }
      }
   
      void pnlShapes_mouseClicked(MouseEvent e)
      {
         if (e.isPopupTrigger())
         {
            return;
         }
      
         if (frameMain.mouseOverItem != null)
         {
            if (frameMain.mouseOverItem instanceof SKBaseShape)
            {
               if (e.isControlDown())
                  frameMain.toggleSelectedShape((SKBaseShape)frameMain.mouseOverItem);
               else
                  frameMain.addOnlySelectedShape((SKBaseShape)frameMain.mouseOverItem);
            }
            else
               frameMain.EditConstraint( (SKBaseConstraint)frameMain.mouseOverItem );
         
            return;
         }
      }
   */
      void scrVert_adjustmentValueChanged(AdjustmentEvent e)
      {
      //    doTranslate(0,lastVertScroll-e.getValue());
       //  set_vervalue(scrVert.getValue());
      }
   
      void scrHoriz_adjustmentValueChanged(AdjustmentEvent e)
      {
         //doTranslate(lastHorizScroll-e.getValue(),0);
         //set_horvalue(scrHoriz.getValue());
      }
   
      void cmbScale_itemStateChanged(ItemEvent e)
      {
         //if (e.getStateChange()==e.SELECTED)
            //doScale((Float.parseFloat(cmbScale.getSelectedItem().toString())) / 100);
      }
   
   /**
   *sets the previous bifurcation on the bifurcation window
   */
      void btnPrevious_actionPerformed(ActionEvent e)
      
      { 
         if (currBifurcation > 0)
            setBifurcation(currBifurcation-1);
/*	 for(int i=0; i<Shapes.size(); i++)
	 for(int j=0; j<Shapes.get(i).ConstraintList.size(); j++)
          {                          
//           System.out.println("111 list of "+Shapes.get(i)+" list = "+Shapes.get(i).ConstraintList.s$
             Shapes.get(i).ConstraintList.get(j).doPaint(pnlShapes3d);
          }
*/
         repaint();
         frameMain.repaint();
      //    scrHoriz.setValue(horvalue);
       //  scrVert.setValue(vervalue);
      }
   /**
   *sets the next bifuraction on the bifurcation window
   */
      void btnNext_actionPerformed(ActionEvent e)
      {
         if (currBifurcation < bifurcations.size()-1)
            setBifurcation(currBifurcation+1);	
/*         for(int i=0; i<Shapes.size(); i++)
	 for(int j=0; j<Shapes.get(i).ConstraintList.size(); j++)
          {                          
//           System.out.println("111 list of "+Shapes.get(i)+" list = "+Shapes.get(i).ConstraintList.s$
             Shapes.get(i).ConstraintList.get(j).doPaint(pnlShapes3d);
          }
*/
         repaint();
         frameMain.repaint();
         //scrHoriz.setValue(horvalue);
         //scrVert.setValue(vervalue);
      
      }
   /**
   *stops the solving process. This method also saves the present state (all the shapes, constraints and groups) for checking whether appropriate update 
   * was made when resolving in the update mode is done
   */
      void btnCancel_actionPerformed(ActionEvent e)
      {
         approved = false;
         //frameMain.allshapes.copyArrayTo(frameMain.oldShapes);
         //frameMain.allConstraints.copyArrayTo(frameMain.oldConstraints);
         frameMain.oldShapeID = frameMain.IDCnt - 1;
         frameMain.oldConstraintID = frameMain.ConstrIDCnt;
         frameMain.oldGroupID = frameMain.GroupIDCnt;
         for(int i=0; i<frameMain.oldConstraints.size(); i++)
         {
            if(frameMain.oldConstraints.get(i) instanceof SKDistanceConstraint)
               frameMain.consArray[frameMain.oldConstraints.get(i).ID] = ((SKDistanceConstraint)frameMain.oldConstraints.get(i)).distance;
            else if(frameMain.oldConstraints.get(i) instanceof SKAngleConstraint)
               frameMain.consArray[frameMain.oldConstraints.get(i).ID] = ((SKAngleConstraint)frameMain.oldConstraints.get(i)).angle;
         }
         hide();
      }
   	/**
   *sets the current bifurcation as the selected bifurcation. The number of this bifurcation is written to the arrays sent to the UTU for further solving
   */
      void btnConfirm_actionPerformed(ActionEvent e)
      {
         drawShapesOnMain();
         for (int i=0; i<Shapes.size();i++)
         {
            SKBaseShape3d sh =Shapes.get(i);
            int id = sh.ID;
            if(!(sh instanceof SKPointSubShape3d))
            {SKBaseShape3d sh1 = frameMain.allshapes3d.findByID(id);
               if( sh1 instanceof SKNormalShape3d)sh1.fixed=true;
               else 
                  for(int a=0; a<sh1.getNumSubShapes()+1; a++)
                     sh1.getSubShape(a).fixed=true;
            }
         }
      
         frameMain.mode=2;
         frameMain.IntArray[0]=2;
         frameMain.solvingShapes3d.clear();
         frameMain.RefreshShapeArea();
         hide();
      }
      void btnResolve_actionPerformed(ActionEvent e)
      
      
      {
         hide();
         reSolveDialog = new JDialog(frameMain,"Re-solve",true);
         final JTextField cluster = new JTextField();
         JTextArea text = new JTextArea("Enter the cluster ID of the cluster you want to re-solve");
         text.setBackground(Color.lightGray);
         JPanel okPanel = new JPanel();
         JButton okBtn = new JButton();
         okBtn.setText("OK");
         okBtn.addActionListener(
                              
                                 new java.awt.event.ActionListener()
                                 
                                 {
                                 
                                 
                                    public void actionPerformed(ActionEvent e)
                                    
                                    {
                                       reSolveID = java.lang.Integer.parseInt(cluster.getText());
                                       reSolveDialog.hide();
                                    }
                                 });
         reSolveDialog.setSize(250,100);
         reSolveDialog.setLocation(691,420);
         reSolveDialog.getContentPane().add(text,BorderLayout.NORTH);
         reSolveDialog.getContentPane().add(cluster,BorderLayout.CENTER);
         reSolveDialog.getContentPane().add(okPanel,BorderLayout.SOUTH);
         okPanel.add(okBtn);
         reSolveDialog.show();
      }
   
   /////////////////////////////////////////////////////
   //
   //  Functionality
   //
   /////////////////////////////////////////////////////
   
   /*      public void doScale(float newScale)
      {
         float delta = newScale/lastScale;
         Component cmp;
         for(int i=0; i<pnlShapes.getComponentCount(); i++)
         {
            cmp = pnlShapes.getComponent(i);
            cmp.setLocation( java.lang.Math.round(cmp.getX()*delta), java.lang.Math.round(cmp.getY()*delta) );
         }
      
         lastScale = newScale;
      }
   
      public void doTranslate(int deltaX, int deltaY)
      {
         //Component cmp;
         SKBaseShape cmp;
         for (int i=0; i<pnlShapes.getComponentCount(); i++)
         {
            cmp = (SKBaseShape)pnlShapes.getComponent(i);
            //System.out.println("before "+cmp+" "+cmp.getX()+" "+cmp.getY()+" "+deltaX+" "+deltaY);
            if(cmp instanceof SKNormalShape)cmp.setLocation(cmp.getX()+deltaX,cmp.getY()+deltaY);
            else 
               for(int a=1; a<cmp.getNumSubShapes()+1; a++)
                  cmp.setShapePoint(new Point(cmp.getX()+deltaX,cmp.getY()+deltaY));
            //System.out.println("after "+cmp+" "+cmp.getX()+" "+cmp.getY());
         }
         pnlShapes.repaint();
      
         if (deltaX != 0)  lastHorizScroll = lastHorizScroll - deltaX;
         if (deltaY != 0)  lastVertScroll = lastVertScroll - deltaY;
         repaint();
      }
   
      void set_vervalue(int i)
      {
         if(i!=0)vervalue = i;
      }
   
      void set_horvalue(int i)
      {
         if(i!=0)horvalue = i;
      }*/
   	/**
   *Displays the current bifurcation in the bifurcation window. This method reads the data required for the display from the bifurcation array using the proper 
   *index.
   */
      public void setBifurcation(int index)
      {
         if (currBifurcation == -1)
         { //Init display
            frameMain.clearSelectedShapes(true,true);
            for (int i=0; i<pnlShapes3d.root_group.numChildren(); i++)
            {
               SKBaseShape s = getShape(pnlShapes3d.root_group.getChild(i).getUserData());
            //frameMain.addSelectedShape(s.clonedShape);
            }
         }
         //System.out.println("111 changed bifur "+index+" "+currBifurcation+" "+bifurcations.size());
      
         if (index != currBifurcation && index < bifurcations.size())
         {
            //System.out.println("111 changed bifur "+index+" "+currBifurcation+" "+bifurcations.size());
         
            cmbScale.setSelectedIndex(1);
            bifurcations.updateShapes(index,this);
	
            for(int i=0; i<pnlShapes3d.root_group.numChildren(); i++)
            {
               SKBaseShape3d sh =null; 
               if(getShape3d(pnlShapes3d.root_group.getChild(i).getUserData())!=null)
	       {
		  sh=getShape3d(pnlShapes3d.root_group.getChild(i).getUserData());
                  for(int j=1; j<sh.getNumSubShapes()+1;j++)
                    sh.getSubShape(j).repaint();
		}
            }
            pnlShapes3d.repaint();
            currBifurcation = index;
            lblStatus.setText(" Bifurcation "+(currBifurcation+1)+" / "+bifurcations.size()+" ");
         }

	   double minX, minY, maxX, maxY, minZ, maxZ;
           maxX = -999;
           minX = 999;
           maxY = -999;
           minY = 999;
           minZ = 999;
           maxZ = -999;
       
           for(int i=0; i<Shapes.size(); i++)
            for(int j=1; j<Shapes.get(i).getNumSubShapes()+1;j++)
            {
             SKBaseShape3d shape = Shapes.get(i).getSubShape(j);
             if(minX>shape.getDX()) minX = shape.getDX();
             if(maxX<shape.getDX()) maxX = shape.getDX();
             if(minY>shape.getDY()) minY = shape.getDY();
             if(maxY<shape.getDY()) maxY = shape.getDY();
             if(minZ>shape.getDZ()) minZ = shape.getDZ();
             if(maxZ<shape.getDZ()) maxZ = shape.getDZ();
             System.out.println("111 "+shape.getDX()+" "+shape.getDY()+" "+shape.getDZ());
            }
            double[] minMax = new double[6];
            minMax[0] = 999; 
            minMax[1] = 999;
            minMax[2] = 999;
            minMax[3] = -999;
            minMax[4] = -999;
            minMax[5] = -999;
            //SKUTUFile.centralize(minX, minY, minZ, maxX, maxY, maxZ, Shapes, minMax);
            minX = minMax[0];
            minY = minMax[1];
            minZ = minMax[2];
            maxX = minMax[3];
            maxY = minMax[4];
            maxZ = minMax[5];
            SKUTUFile.doScale(Shapes);
            System.out.println("111 doscale");



         SKBifurcation bifur=bifurcations.get(currBifurcation);
         frameMain.RefreshShapeArea();
         for(int i=0; i<frameMain.allshapes.size(); i++)
            frameMain.allshapes.get(i).repaint();
      }
   /**
   *This method displays the partially solved sketch on the main window as and when the user picks the bifurcation
   */
      private void drawShapesOnMain()
      
      {
         int maxX=0, maxY=0, tempX, tempY, count = 0,x,y;
         SKBaseShape3d fix=null,fix1=null,tip = null;
         SKShape3dArray fixedShapes = new SKShape3dArray(10);
         SKShape3dArray allSubShapes = new SKShape3dArray(50);
         SKShape3dArray solvingShapes = new SKShape3dArray(50);
         SKShape3dArray nonSolvingShapes = new SKShape3dArray(50);
         JOptionPane dialog = null;
      
         for(int i=0; i<frameMain.panelShapeArea3d.root_group.numChildren(); i++)
         { 
            if(frameMain.panelShapeArea3d.root_group.getChild(i).getUserData() != null)
            {
               SKBaseShape3d temp = null;
	       if(getShape3d(frameMain.panelShapeArea3d.root_group.getChild(i).getUserData())!=null)
	       { temp = getShape3d(frameMain.panelShapeArea3d.root_group.getChild(i).getUserData());
               allSubShapes.add(temp);
               if(temp.fixed)
                  fixedShapes.add(temp);
	       }
            }
         }
      
         allSubShapes.copyArrayTo(nonSolvingShapes);
         for(int i=0; i<pnlShapes.getComponentCount(); i++)
         {
            for(int k=0; k<allSubShapes.size(); k++)
               if(allSubShapes.get(k).ID == ((SKBaseShape)pnlShapes.getComponent(i)).ID)
               {
                  nonSolvingShapes.removeShape(allSubShapes.get(k));
                  solvingShapes.add(allSubShapes.get(k));
               }
            for(int k=0; k<fixedShapes.size(); k++)
            {
               SKBaseShape3d temp = null;
		if(getShape3d(pnlShapes3d.root_group.getChild(i).getUserData())!=null)
		{temp = getShape3d(pnlShapes3d.root_group.getChild(i).getUserData());
               if(fixedShapes.get(k).ID == temp.ID)
               {  
                  fix = temp;
                  count++;
               }
	       }
            }
         }
      
         if(count<=1)
         {
            for(int i=0; i<frameMain.panelShapeArea3d.root_group.numChildren(); i++)
            {
               if(getShape3d(frameMain.panelShapeArea3d.root_group.getChild(i).getUserData()) != null)
               {
                  SKBaseShape3d t = getShape3d(frameMain.panelShapeArea3d.root_group.getChild(i).getUserData());
                  if(maxX<=t.getX())
                  {
                     tip = t;
                     maxX = t.getX();
                  }
               }	
            }
            if(count ==0)
            {
               fix = getShape3d(pnlShapes3d.root_group.getChild(0).getUserData());
               for(int i=0; i<pnlShapes3d.root_group.numChildren(); i++)
  		if( getShape3d(pnlShapes3d.root_group.getChild(i).getUserData())!=null)             
                  if(fix.getX()>getShape3d(pnlShapes3d.root_group.getChild(i).getUserData()).getX())
                     fix = getShape3d(pnlShapes3d.root_group.getChild(i).getUserData());
            
               for(int i=0; i<frameMain.panelShapeArea3d.root_group.numChildren(); i++)
               {
                  if( getShape3d(frameMain.panelShapeArea3d.root_group.getChild(i).getUserData()) != null)
                     if(fix.ID == getShape3d(frameMain.panelShapeArea3d.root_group.getChild(i).getUserData()).ID)
                        fix1 = getShape3d(frameMain.panelShapeArea3d.root_group.getChild(i).getUserData());
               }
            }
            else if(count ==1)
            {
               for(int i=0; i<frameMain.panelShapeArea3d.root_group.numChildren(); i++)
                  if(getShape3d(frameMain.panelShapeArea3d.root_group.getChild(i).getUserData()) != null)
                     if(fix.ID == getShape3d(frameMain.panelShapeArea3d.root_group.getChild(i).getUserData()).ID)
                        fix1 = getShape3d(frameMain.panelShapeArea3d.root_group.getChild(i).getUserData());
            }
         
            for (int i=0; i<Shapes.size();i++)
               for(int j=1; j<Shapes.get(i).getNumSubShapes()+1;j++)
               {
                  SKBaseShape3d sh = Shapes.get(i).getSubShape(j);
                  if(sh.ID!=fix.ID)
                  {  SKBaseShape3d sh1,sh1P;
                     int id =Shapes.get(i).ID;
                     SKBaseShape3d temp = Shapes.get(i);
                     if(!(temp instanceof SKPointSubShape3d)) 
                     {sh1 = frameMain.allshapes3d.findByID(id).getSubShape(j);
                        sh1P = frameMain.allshapes3d.findByID(id);}
                     else
                     { sh1=temp;
                        sh1P=sh1;}
                  
                     SKBifurcation bifur=bifurcations.get(currBifurcation);
                     x=(fix.getX()-sh.getX());
                     y=(fix.getY()-sh.getY());
                     if(sh1.ID!=fix1.ID && !sh1.fixed)
                        sh1.setShapePoint(new Point3d((fix1.getX()-x),(fix1.getY()-y),0));
                     sh1P.repaint();
                  }
               }
            boolean noOverlap;
            noOverlap = checkRotation(fix1, nonSolvingShapes, solvingShapes);
         
            int cnt=0;
            while((cnt<20) && (!noOverlap))
            {
               if(!fix1.fixed) 
               {
                  fix1.setShapePoint(new Point3d(fix1.getX()+5,fix1.getY(),0));
                  noOverlap = checkRotation(fix1, nonSolvingShapes, solvingShapes);
               }
               cnt++;
            }
         }
      }
   	/**
   *This method attempts rotate the partially solved sketchs so that they do not overlap
   */
      public boolean checkRotation(SKBaseShape3d fix1, SKShape3dArray nonSolvingShapes, SKShape3dArray solvingShapes)
      {
         int p1x, p2x, p1y,p2y,px=0,py=0;
         int[] points = new int[4];
         System.out.println("convex Hull");
         convexHull(fix1,solvingShapes,points);
         p1x=points[0];
         p1y=points[1];
         p2x=points[2];
         p2y=points[3];
         boolean outside=false;
         double rad=0;
         while(!outside && (rad<=java.lang.Math.PI*2))
         {
            outside = true;
            for(int i=0; i<nonSolvingShapes.size(); i++)
            {
               px = nonSolvingShapes.get(i).getX();
               py = nonSolvingShapes.get(i).getY();
               outside = outside && (((py-p1y)*(p2x-p1x))-((px-p1x)*(p2y-p1y)) < 0);
            }
            if(!outside)
            {
               float r = distance(p1x,p1y,p2x,p2y);
               rad = rad+(java.lang.Math.PI/36);
               AffineTransform T = new AffineTransform();
               T.setToRotation(rad,fix1.getX(),fix1.getY());
            
               float[] src,dst;
            
               src = new float[2];
               dst = new float[2];
               src[0]=p2x;
               src[1]=p2y;
               T.transform(src,0,dst,0,1);
               p2x=(int)dst[0];
               p2y=(int)dst[1];
            
               for(int i=0; i<solvingShapes.size(); i++)
               {SKBaseShape3d sol = solvingShapes.get(i);
                  if(!sol.fixed && sol.ID!=fix1.ID)
                  {
                     px = sol.getX();
                     py = sol.getY();
                     src[0]=px;
                     src[1]=py;
                     T.transform(src,0,dst,0,1);
                     SKBaseShape3d sh;
                     for(int k=0; k<frameMain.allshapes3d.size(); k++)
                        for(int j=1; j<frameMain.allshapes3d.get(k).getNumSubShapes()+1; j++)
                           if(sol.ID == frameMain.allshapes3d.get(k).getSubShape(j).ID) 
                           {
                              sh=frameMain.allshapes3d.get(k).getSubShape(j);
                              sh.setShapePoint(new Point3d((int)dst[0],(int)dst[1],0));
                                    //System.out.println("set "+sh+" "+px+" "+py);
                           }
                  }
               }
               System.out.println(rad);
            }
         }
      
         if(rad<=java.lang.Math.PI*2)
            return true;
         else 
            return false;
      }
   
      public static float distance(int x1, int y1, int x2, int y2)
      {
         return (float)java.lang.Math.sqrt(java.lang.Math.pow((double)(x1-x2),2)+java.lang.Math.pow((double)(y1-y2),2));
      }
      public void convexHull(SKBaseShape3d fix1, SKShape3dArray Shapes, int points[])
      {
         int p1x=fix1.getX(), p1y=0, p2x=fix1.getX(), p2y=fix1.getY()+300, px=0, py=0,l=0;
         boolean  outside = false;
         SKShape3dArray shapes= new SKShape3dArray(2);
         SKShape3dArray temp= new SKShape3dArray(2);
         Shapes.copyArrayTo(temp);
         temp.removeShape(fix1);
         while((!outside) && (l<temp.size()))
         {
            outside = true;
            SKBaseShape3d tempSh = temp.get(l);
            double slope =(double) (fix1.getY() -  tempSh.getY())/(double)(fix1.getX() - tempSh.getX());
            double c = fix1.getY() -(slope * fix1.getX());
            if(fix1.getX() == tempSh.getX())
            {
               p1y=0;
               p2y=300;
               p1x= (int)((p1y - c) /slope);
               p2x= (int)((p2y - c) /slope);
            }
            else
            {
               p1x=0;
               p2x=300;
               p1y= (int)((slope * p1x) +c);
               p2y= (int)((slope * p2x) +c);
            }
            temp.copyArrayTo(shapes);
            shapes.removeShape(tempSh);
            for(int i=0; i<shapes.size(); i++)
            {
               px = shapes.get(i).getX();
               py = shapes.get(i).getY();
               outside = outside && (((py-p1y)*(p2x-p1x))-((px-p1x)*(p2y-p1y)) < 0);
            }
            l++;
            System.out.println("convex hull "+tempSh+" "+outside);
         }
         points[0] = p1x;
         points[1] = p1y;
         points[2] = p2x;
         points[3] = p2y;
      }
      public SKBaseShape getShape(Object data)
      {     
         String name = (String)data;
         for(int i=0; i<frameMain.allshapes.size(); i++)
         {
            if(name.equals(frameMain.allshapes.get(i).Name))
               return frameMain.allshapes.get(i);
         }
         return null;
      }  
      public SKBaseShape3d getShape3d(Object data)
      {
         String name = (String)data;
	 if(name != null)
         for(int i=0; i<frameMain.allshapes3d.size(); i++)
            if(name.equals(frameMain.allshapes3d.get(i).Name))
               return frameMain.allshapes3d.get(i);
         return null;
      }    
       public void cloneConstraints(SKShape3dArray arrShapes)
      {
         arrCon = new SKConstraint3dArray(1);
         for(int i =0; i<arrShapes.size(); i++)
         {   
           SKBaseShape3d sh = arrShapes.get(i);
           SKBaseShape3d mainSh = null;
           for(int j=0; j<frameMain.allshapes3d.size(); j++)
           {
                if(sh.ID == frameMain.allshapes3d.get(j).ID)
                {  mainSh = frameMain.allshapes3d.get(j);
                   break;
                }
           } 
           if(mainSh != null)
           {
             for(int j=0; j<mainSh.ConstraintList.size(); j++)
             {
                SKBaseConstraint3d mainCon = mainSh.ConstraintList.get(j);
                boolean present = true;
                for(int k=0; k<mainCon.ShapeList.size(); k++)
                  if(arrShapes.findByID(mainCon.ShapeList.get(k).ID) == null)
                    {present = false;
                     break;}
                if(present && (arrCon.findByID(mainCon.ID)==null))
                {
		  SKBaseConstraint3d cloneCon = frameMain.createCloneConstraint3d(mainCon,arrShapes);
		  if(mainCon instanceof SKDistanceConstraint3d)
			cloneCon.changeColor(((SKDistanceConstraint3d)mainCon).color);
		  pnlShapes3d.add(cloneCon);
                  arrCon.add(cloneCon);
                }
             }
           }
         }
         System.out.println("111 cons size = "+arrCon.size());
      }

   }

