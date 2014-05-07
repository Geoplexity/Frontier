/**
 * Title:        SKBaseShape
 * Description:  Base class for all Shapes used with Sketcher
 */
   package sketcher;

   import javax.swing.JComponent;
   import java.awt.Graphics;
   import java.awt.Cursor;
   import java.awt.Point;
   import java.awt.Color;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;
   import java.awt.event.*;

//This is the base class for all shapes to be implemented as GUI-drawn shapes
   public class SKBaseShape extends JComponent implements SKItemInterface
   {
   //Functionality
      public static SKMainFrame frameMain;
      public boolean   Selected  = false;     //Draws selected rect if true
      public int       DragState = -1;        //State of dragging, -1=not, 0=move, 1=NW resize, 2=SE resize
      public int       DragX;
      public int       DragY;
      public int       GroupID   = -1;
      public SKConstraintArray ConstraintList = new SKConstraintArray(1); //List of constraints
      public Color     bodyColor;
      public Color     outlineColor;
      public SKGroupTreeNode highestGroup = null;
      public SKBaseShape selShape = this; //Lets you know what part of an object with SubShapes was actually clicked causing the shape to be selected
   
   //Properties
      public int       ID;                    //This is the unique identifier associated with this shape
      public int       repID=0;
      public String    ShapeType;             //Type of shape
      public int       ShapeTypeID;           //Type ID of shape
      public String    Name;                  //Name of this shape (for users preference)
      public String    IconPath;              //Generic shape icon (for cmbShapes)
      public boolean   isClone;               //True if this object is a clone (created by .cloneShape() )
      public SKBaseShape  clonedShape = null; //Shape this was cloned to
      public boolean   fixed = false;  
      public int       orientCons;            //# of parallel or perpendicular constraints
      public int       toServeConstraint;     //parallel or perpendicular constraint
      public SKShapeArray clusterShapes = new SKShapeArray(2);
   
      public SKBaseShape(SKMainFrame frame, int idnum, String newname, String shapeType, int shapeTypeID)
      {
         frameMain = frame;
         ID = idnum;
         Name = newname;
         ShapeType = shapeType;
         ShapeTypeID = shapeTypeID;
      }
      public int getShapeTypeID()
      {//System.out.println("base "+ShapeTypeID);
         return ShapeTypeID;
      }
   
      public int getID()
      {
         return ID;
      }
   
   //Severs all ties (for gc)...called when deleting shape
      public void severTies()
      {
         if (!isClone)
            while (ConstraintList.size() > 0)
               removeConstraint( ConstraintList.get(0) );
      
         ConstraintList = null;
         //frameMain = null;
         highestGroup = null;
      }
   
   //Sets colors for rendering shape (based on user's options and current states)
      public void updateColors()
      {//System.out.println("entered update color");
         SKBaseShape sh;
      
         if (isClone)
            sh = clonedShape;
         else
            sh = this;
         if (frameMain.selectedGroupShapes.indexOf(sh.getSelectable()) > -1)
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.grpShapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.grpShapeOutlineColor];
         }
         if (sh.Selected)
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.selShapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.selShapeOutlineColor];
         }
         else if (sh == frameMain.mouseOverItem)
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.moShapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.moShapeOutlineColor];
         }
         else if (sh.fixed)
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.fixShapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.fixShapeOutlineColor];
         }
         else
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.shapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.shapeOutlineColor];
         }
         //System.out.println(ID+" "+fixed+" "+bodyColor);
      }
   
   //Should this shape be drawn?
      public boolean shouldDraw()
      {   
         return (!frameMain.drawGroupRects || frameMain.selectedGroupShapes.indexOf(this) > -1);
      }
   
   //Called when panelShapeArea is redrawn to paint constraints (ie lines between objects)
      public void paintConstraints(Graphics g)
      {
      //Paint constraints (if we are primary shape)
         SKBaseConstraint con;
         for (int i=0; i<ConstraintList.size(); i++)
         {
            con = ConstraintList.get(i);
            if (con.isPrimaryShape(this)) con.doPaint(g);
         }
      }
   
   //This is to allow shapes to paint outside of thier boundaries (like line between points for SKLineShape)
      public void paintSpecial(Graphics g)
      { }
   
   //Sets SKMainFrame's vCurrPropNames and vCurrPropData and refreshes tableShapeProps
   //All descendents that implement new properties MUST override this (but still call super.updateMainPropData()
      public void updateMainShapeData(SKPropArray vPropList, SKConstraintArray vCurrentConstraints)
      {
         if (vPropList != null)
         { //Update properties
            frameMain.propFactory.freeProps(vPropList);
            vPropList.add(frameMain.propFactory.getNewProp("ID", Integer.toString(ID),0,false));
            vPropList.add(frameMain.propFactory.getNewProp("Type",ShapeType,0,false));
            vPropList.add(frameMain.propFactory.getNewProp("Name",Name,0));
            vPropList.add(frameMain.propFactory.getNewProp("Group", Integer.toString(GroupID),0,false));
         }
      
         if (vCurrentConstraints != null)
         { //Update constraints
            vCurrentConstraints.clear();
            ConstraintList.copyArrayTo(vCurrentConstraints);
         }
      }
   
   //Sets this component's properties based on the values in the data vectors
   //returns false if PropName wasn't handled
      public boolean setShapeData(String PropName, Object PropData)
      {
         if (PropName=="Name")
         {
            Name = (String)PropData;
         }
         else if (PropName=="Group")
         {
            GroupID = new Integer(PropData.toString()).intValue();
         }
         else 
            return false;
         return true;
      }
   
   //Add all properties that should be listed for this shape and this constraint (when editing constraint)
      public void updateConstraintProps(SKBaseConstraint con, SKPropArray vPropList)
      { }
   
   //Called when user has changed constraint prop data to get info to store
      public int getConPropInfo(SKBaseConstraint con, String PropName, Object PropData)
      {
         return 0;
      }
   
      public boolean isSelected()
      {
         return Selected;
      }
   
      public void setSelected(boolean sel)
      {
         if (sel != Selected)
         {
            Selected = sel;
            selShape = this;
            repaint();
         }
         setCursor( Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR) );
      }
   
      public void addConstraint(SKBaseConstraint con)
      {
         con.addShape(this);
      }
   
      public void removeConstraint(SKBaseConstraint con)
      {
         con.removeShape(this);
      }
   
      public String toString()
      {
         return Name;
      }
   
      public int getShapeX()
      {
         return getX()+(getWidth()>>1);
      
      }
   
      public int getShapeY()
      {
         return getY()+(getHeight()>>1);
      }
      public void setShapePoint(Point pt)
      {}
   //Returns what should go into SelectedShapes array (if this is a SubShape, its parent shape is returned)
      public SKBaseShape getSelectable()
      {
         return this;
      }
   
   //Moves this shape and (if applicable) other shapes
      public void doMove(int deltaX, int deltaY, boolean resolveConstraints)
      {
         if (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0 && resolveConstraints)
         {
            if(SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] == 0)
               SKSimpleSolver.ResolveSimpleConstraints(this.getSelectable(),new SKConstraintArray(3));
            else 
               SKSimpleSolver.ResolveSimpleConstraints(this.getSelectable());
         }
      }
   
   //Number of shapes involed in the given constraint
      public int getNumConstraintPoints(SKBaseConstraint con)
      {
         return 1;
      }
   	//Number of SubShapes
      public int getNumSubShapes()
      {
         return 0;
      }
   
   //Used to access a complex shape's SubShapes
      public SKBaseShape getSubShape(int index)
      {
         return this;
      }
   
   //Called by constraints to get point to draw from/to
      public Point getConstraintPoint(SKBaseConstraint con, int mod)
      {
         return new Point(getShapeX(),getShapeY());
      }
      //Called by constraints to get point to draw from/to
   
      public Point notConstraintPoint(SKBaseConstraint con, int mod)
      
      {
         return new Point(getShapeX(),getShapeY());
      }
   
   //Allows objects with SubShapes to set their positions
      public void setConstraintPoint(SKBaseConstraint con, int mod, Point pt)
      
      {
         setShapePoint(pt);
      }
   
   //Allows a constraint to access its shape's SubShapes by mod
      public int getConstraintMod(SKBaseShape sh)
      {
         return 0;
      }
   
   //Used when calculating distance for display
      public Point getPointForDistance(SKBaseShape otherShape)
      {
         return new Point(getShapeX(),getShapeY());
      }
   
   //Determines if the given x,y are over the shape (only for shapes that drawn outside thier boundries using paintSpecial()
   //This should be overriden by shapes that draw outside of their boundaries (ie SKLineShape)
      public boolean doHitTest(int x, int y)
      {
         return false;
      }
   
   //Writes shape's data to a data stream
      public void writeToStream(DataOutputStream p, boolean writeID) throws IOException
      {
      //Write this object's info
         p.writeInt(ShapeTypeID);
         if (writeID)  
            p.writeInt(ID);
         else p.writeInt(repID);
         p.writeUTF(Name);
      
         writeAdditionalProps(p);
      }
   
   //Override this to add persistent properties
      public void writeAdditionalProps(DataOutputStream p) throws IOException
      { }
   
   //Writes all of this shape's constraints to data stream
      public void writeConstraintsToStream(DataOutputStream p, boolean writeID, SKShapeArray shapes) throws IOException
      {
      //Write associated constraints to stream if this is primary shapes
         SKBaseConstraint con;
         for (int i=0; i<ConstraintList.size(); i++)
         {
            con = ConstraintList.get(i);
            if (con.isPrimaryShape(this)) con.writeToStream(p,writeID,shapes);
         }
      }
   
   //Reads properties from stream
      public void readFromStream(DataInputStream p) throws IOException
      {
      //Read and set properties
         Name = p.readUTF();
      
         readAdditionalProps(p);
      }
      public void readFromStream(DataInputStream p, int x, int y) throws IOException
      
      {
      //Read and set properties
         Name = p.readUTF();
         if(this instanceof SKPointShape)
            Name = "Point"+ID;
         else if(this instanceof SKLineShape)
            Name = "Line"+ID;
         else if(this instanceof SKCircleShape)
            Name = "Circle"+ID;
         else if(this instanceof SKArcShape)
            Name = "Arc"+ID;
         readAdditionalProps(p,x,y);
      }
   //Override this to read additional persistent properties (from writeAdditionalProps)
      public void readAdditionalProps(DataInputStream p) throws IOException
      { }
   
      public void readAdditionalProps(DataInputStream p,int x,int y) throws IOException
      
      { }
   
   //Used for non-normal shapes (so only saved once)
      public boolean isPrimaryShape(SKBaseShape sh)
      {
         return true;
      }
   
      public void updateFromUTU(char vr, char mod, double deltaX, double deltaY, double value)
      {}
   
   //Updates this shape data from Bifurcation data
      public int bifurUpdate(float[] data, int startIdx, int addX, int addY, float scale)
      {
         return startIdx;
      }
   
      public int bifurDataCount()
      {
         return 0;
      }
   
      public void cloneShape(SKBaseShape sh)
      {
         sh.ID = ID;
         sh.Name = Name;
         sh.ShapeType = ShapeType;
         sh.ShapeTypeID = ShapeTypeID;
         sh.isClone = true;
         sh.clonedShape = this;
         clonedShape = sh;
        // System.out.println("constraint list "+ConstraintList.size());
         for (int i=0; i<ConstraintList.size(); i++)
         {
            sh.ConstraintList.add( ConstraintList.get(i) );
            ConstraintList.get(i).ShapeList.add(sh);
         }
      }
   
      public SKItemInterface getClonedItem()
      {
         return clonedShape;
      }
   
      public  void frameUpdate(float[] data, int startIdx, int addX, int addY, float scale)
      
      {
         setShapePoint(new Point((int)addX,(int)addY));
      }
      public void addMouseHandlers()
      {
         addMouseListener( 
                         
                            new MouseListener()
                            {
                               public void mouseClicked(MouseEvent e)
                               {
                                  SKBaseShape sh = (SKBaseShape)e.getComponent();
                                  if (e.isControlDown())
                                  { //Multi select
                                     frameMain.toggleSelectedShape(sh);
                                  }
                                  else
                                  {
                                     if (!sh.isSelected()) frameMain.addOnlySelectedShape(sh);
                                  }
                               }
                            
                               public void mouseReleased(MouseEvent e)
                               {
                                  SKBaseShape sh = (SKBaseShape)e.getComponent();
                                  if (e.isPopupTrigger())
                                  {
                                     if ( frameMain.SelectedShapes.size()==0 ) frameMain.addSelectedShape(sh);
                                     frameMain.popupShape.show(sh,e.getX(),e.getY());
                                  }
                                  else
                                  {
                                     if ( !sh.isSelected() ) 
                                        return;
                                     if (sh.DragState != -1)
                                     {
                                              //Do SimpleSolver
                                        if (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]==1)
                                        {
                                           if (frameMain.allSelNeedsResolved)
                                           {
                                              frameMain.allSelNeedsResolved = false;
                                           
                                              for (int i=0; i<frameMain.SelectedShapes.size(); i++)
                                              { frameMain.SelectedShapes.get(i).doMove(0,0,true);
                                              
                                              }
                                           }
                                           else sh.doMove(0,0,true);
                                        }
                                        sh.DragState = -1;
                                        sh.getSelectable().updateMainShapeData(frameMain.vCurrentProps,null);
                                        frameMain.tableObjectProp.repaint();
                                     
                                        if (sh.getSelectable().highestGroup != null)
                                           sh.getSelectable().highestGroup.groupRect.w = 0;
                                     
                                        sh.repaint();
                                     }
                                  }
                               }
                            
                               public void mousePressed(MouseEvent e) {
                               }
                            
                               public void mouseExited(MouseEvent e) {
                               }
                            
                               public void mouseEntered(MouseEvent e) {
                               }
                            } );
         addMouseMotionListener( 
                              
                                 new MouseMotionListener()
                                 {
                                 
                                    public void mouseDragged(MouseEvent e)
                                    {SKBaseShape sh = (SKBaseShape)e.getComponent();
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
                                             boolean doSimpleSolve = (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]==2);
                                             switch (sh.DragState)
                                             {
                                                case 0 : 
                                                   if (e.isAltDown())
                                                   { //Move all selected shapes relative to sh
                                                      for (int i=0; i<frameMain.SelectedShapes.size(); i++)
                                                      { frameMain.SelectedShapes.get(i).doMove(e.getX()-sh.DragX,e.getY()-sh.DragY,doSimpleSolve);
                                                      }
                                                      frameMain.allSelNeedsResolved = !doSimpleSolve;
                                                   }
                                                   else
                                                   { //Just move sh
                                                      sh.doMove(e.getX()-sh.DragX,e.getY()-sh.DragY,doSimpleSolve);
                                                   }
                                             }
                                             frameMain.RefreshShapeArea();
                                          }
                                          if (frameMain.SelectedShapes.size() == 2)
                                          {
                                             Point pt1 = frameMain.SelectedShapes.get(0).getPointForDistance(frameMain.SelectedShapes.get(1)),
                                             pt2 = frameMain.SelectedShapes.get(1).getPointForDistance(frameMain.SelectedShapes.get(0));
                                             frameMain.sbStatus.updatePanelText(Float.toString((float)pt1.distance(pt2)),2);
                                          }
                                                  /*
                                                  if (sh.DragState==-1)
                                                  { //Dragging -- show what X,Y for shape would be
                                                    frameMain.sbStatus.updatePanelText((e.getX()+sh.getX()-sh.DragX)+" , "+(e.getY()+sh.getY()-sh.DragY),2);
                                                  }
                                                  else
                                                  { //Other - show real X,Y
                                                    frameMain.sbStatus.updatePanelText((sh.getX()+(sh.getWidth()>>1))+" , "+(sh.getY()+(sh.getHeight()>>1)),2);
                                                  }
                                                  */
                                       }
                                    }
                                 
                                    public void mouseMoved(MouseEvent e)
                                    { //Set correct mouse cursor
                                       SKBaseShape sh = (SKBaseShape)e.getComponent();
                                                //frameMain.sbStatus.updatePanelText(e.getX()+sh.getX()+" , "+(e.getY()+sh.getY()),2);
                                       frameMain.sbStatus.updatePanelText(sh.getSelectable().Name,3);
                                       if (sh.getSelectable() == sh && frameMain.mouseOverItem != sh)
                                       {
                                          frameMain.mouseOverItem = sh;
                                          frameMain.RefreshShapeArea();
                                       }
                                       sh.setCursor( Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR) );
                                    }
                                 } );
      
      }
   }