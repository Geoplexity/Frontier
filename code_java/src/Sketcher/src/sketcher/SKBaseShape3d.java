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

   import javax.media.j3d.*;
   import javax.media.j3d.SceneGraphObject;
   import javax.media.j3d.Shape3D;
   import javax.vecmath.*;

//This is the base class for all shapes to be implemented as GUI-drawn shapes
   public class SKBaseShape3d extends TransformGroup
   {
   //Functionality
      public static SKMainFrame frameMain;
      public boolean   Selected  = false, fixed = false, clicked = false;     //Draws selected rect if true
      public Matrix3f matrix = new Matrix3f();
      public Vector3f vector = new Vector3f();
      public int       DragState = -1;        //State of dragging, -1=not, 0=move
      //public int       DragX;
      //public int       DragY;
      public int       GroupID   = -1;
   
   
      public  java.net.URL red = null;
      public  java.net.URL blue = null;
      public  TextureUnitState textureUnitState[] = new TextureUnitState[2];
      public  Texture redTex;
      public  Texture blueTex;
   
      public SKBaseShape3d selShape = this; //Lets you know what part of an object with SubShapes was actually clicked causing the shape to be selected
      public float X,Y,Z;
      public int h,w;
   //Properties
      public int       ID;                    //This is the unique identifier associated with this shape
      public String    ShapeType;             //Type of shape
      public int       ShapeTypeID;           //Type ID of shape
      public String    Name;                  //Name of this shape (for users preference)
      public String    IconPath;              //Generic shape icon (for cmbShapes)
      public boolean   isClone;               //True if this object is a clone (created by .cloneShape() )
      public SKBaseShape3d  clonedShape = null; //Shape this was cloned to
      public SKConstraint3dArray ConstraintList = new SKConstraint3dArray(2);
      public SKGroupTreeNode highestGroup = null;
   
      public SKBaseShape3d(SKMainFrame frame, int idnum, String newname, String shapeType, int shapeTypeID)
      {
	 super();
         frameMain = frame;
         ID = idnum;
         Name = newname;
         ShapeType = shapeType;
         ShapeTypeID = shapeTypeID;
         this.setCapability(Shape3D.ALLOW_APPEARANCE_WRITE);
         this.setUserData(Name);
      }
      public int getShapeTypeID()
      {//System.out.println("base "+ShapeTypeID);
         return ShapeTypeID;
      }
      public Point3d getPointForDistance(SKBaseShape otherShape)
      
      {
         return new Point3d(getX(),getY(),getZ());
      }
      public int getID()
      {
         return ID;
      }
      public int getHeight()
      {
         return h;
      }
      public int getWidth()
      {
         return w;
      }
      public void doMove(Vector3f v)
      {
	//System.out.println("111 moving "+this+" "+ConstraintList.size());
	//System.out.println ( "Drawing the constraints...  " + ConstraintList.size());
	for(int i=0; i<ConstraintList.size(); i++)
	    {
	      ConstraintList.get(i).doPaint();
	      //System.out.println("111 drawing "+ConstraintList.get(i));
	    }
      }
      public void doMove()
      {
	for(int i=0; i<ConstraintList.size(); i++)
	  if(!ConstraintList.get(i).create)
	     ConstraintList.get(i).doPaint();
	}
   //Severs all ties (for gc)...called when deleting shape
      public void severTies()
      {
      }
      public void changeColor(Color c)
      {
      }   
      public void changeColor(Color3f c)
      {
      }   
   //Sets colors for rendering shape (based on user's options and current states)
      public void updateColors()
      {//System.out.println("entered update color");
         /*SKBaseShape sh;
      
         if (isClone)
            sh = clonedShape;
         else
            sh = this;
      
         if (sh.Selected)
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.selShapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.selShapeOutlineColor];
         }
         else
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.shapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.shapeOutlineColor];
         }
         //System.out.println(ID+" "+fixed+" "+bodyColor);*/
      }
   
   //Should this shape be drawn?
      public boolean shouldDraw()
      {
         //return (!frameMain.drawGroupRects || frameMain.selectedGroupShapes.indexOf(this) > -1);
         return true;
      }
   
   //Called when panelShapeArea is redrawn to paint constraints (ie lines between objects)
      public void paintConstraints(Graphics g)
      {
      //Paint constraints (if we are primary shape)
      }
      public void reshape(int x, int y, int h, int w)
      {
      
      }
   //This is to allow shapes to paint outside of thier boundaries (like line between points for SKLineShape)
      public void paintSpecial(Graphics g)
      { }
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
      
      }
      public void readAdditionalProps(DataInputStream p) throws IOException
      
      { }
      public void writeToStream(DataOutputStream p, boolean writeID) throws IOException
      
      {
      //Write this object's info
         p.writeInt(ShapeTypeID);
         if (writeID)
            p.writeInt(ID);
      
         p.writeUTF(Name);
      
         writeAdditionalProps(p);
      }
   
   //Override this to add persistent properties
   
      public void writeAdditionalProps(DataOutputStream p) throws IOException
      
      { }
   
      public void readAdditionalProps(DataInputStream p,int x,int y) throws IOException
      { }
      public int getConstraintMod(SKBaseShape3d sh)      {
         return 0;
      }
      public int bifurUpdate(float[] data, int startIdx, int addX, int addY, float scale)
      {
         return startIdx;
      }
      public int bifurDataCount()
      {
         return 0;
      }
      public void cloneShape(SKBaseShape3d sh)
      {
         sh.ID = ID;
         sh.Name = Name;
         sh.ShapeType = ShapeType;
         sh.ShapeTypeID = ShapeTypeID;
         sh.isClone = true;
         sh.clonedShape = this;
         clonedShape = sh;
      }
   //Sets SKMainFrame's vCurrPropNames and vCurrPropData and refreshes tableShapeProps
   //All descendents that implement new properties MUST override this (but still call super.updateMainPropData()
      public void updateMainShapeData(SKPropArray vPropList)
      {
         if (vPropList != null)
         { //Update properties
            frameMain.propFactory.freeProps(vPropList);
            vPropList.add(frameMain.propFactory.getNewProp("ID", Integer.toString(ID),0,false));
            vPropList.add(frameMain.propFactory.getNewProp("Type",ShapeType,0,false));
            vPropList.add(frameMain.propFactory.getNewProp("Name",Name,0));
            vPropList.add(frameMain.propFactory.getNewProp("Group", Integer.toString(GroupID),0,false));
         }
      }
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
   
   
   //Called when user has changed constraint prop data to get info to store
   
   
      public boolean isSelected()
      {
         return Selected;
      }
   
      public void setSelected(boolean sel)
      {
         Selected=sel;
         updateColors();
      }
      public void repaint()
      {
         frameMain.panelShapeArea3d.canvas3D.repaint();
      }
   
      public String toString()
      {
         return Name;
      }
   
      public int getX()
      {
         getMatrix();
         return (int)X;
      }
   
      public int getY()
      {
         getMatrix();
         return (int)Y;
      }
      public int getZ()
      {
         getMatrix();
         return (int)Z;
      }
      public double getDX()
      {
         getMatrix();
         return X;
      }
   
      public double getDY()
      {
         getMatrix();
         return Y;
      }
      public double getDZ()
      {
         getMatrix();
         return Z;
      }
      public void getMatrix()
      {}
      public void setShapePoint(Point3d pt)
      {
      
      }
   //Returns what should go into SelectedShapes array (if this is a SubShape, its parent shape is returned)
      public SKBaseShape3d getSelectable()
      {
         return this;
      }
   
   //Moves this shape and (if applicable) other shapes
      public void doMove(int deltaX, int deltaY, boolean resolveConstraints)
      {
      }
      public void doMoveOnPlane(int x, int y)
      
      {      }
      public void setLocation( int x, int y)
      {
      
      }
   //Number of shapes involed in the given constraint
      	//Number of SubShapes
      public int getNumSubShapes()
      {
         return 0;
      }
   
   //Used to access a complex shape's SubShapes
      public SKBaseShape3d getSubShape(int index)
      {
         return this;
      }
   
   //Called by constraints to get point to draw from/to
         //Called by constraints to get point to draw from/to
      public boolean isPrimaryShape(SKBaseShape3d sh)
      
      {
         return true;
      }
      public void updateConstraintProps(SKBaseConstraint con, SKPropArray vPropList)
      { }
      public Point3d getConstraintPoint(SKBaseConstraint3d con, int mod)      {
         return new Point3d(getDX(),getDY(), getDZ());
      }
      public int getConPropInfo(SKBaseConstraint3d con, String PropName, Object PropData)
      {
         return 0;
      }
   
      public static void adjustTransform(Transform3D t)
      
      {
         Vector3f v1=new Vector3f();
         Transform3D view = frameMain.panelShapeArea3d.getView();
         view.get(v1);
         Transform3D rot = frameMain.panelShapeArea3d.getRotation();
         if(frameMain.topView)
            v1.y = v1.y-20.0f;
         else 
            v1.z = v1.z-20.0f;
         Transform3D t1 = new Transform3D();
         t1.setTranslation(v1);
         t1.mul(rot);
         t.mul(t1);
      }
     public Node getAddTrans()
          {
                  return null;
          }

   }
