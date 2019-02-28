/**
 * Title:        SKBaseConstraint
 * Description:  Base class for all constraints
 */
   package sketcher;
   import java.awt.Graphics;
   import java.awt.Graphics2D;
   import java.awt.Shape;
   import java.awt.geom.QuadCurve2D;
   import java.awt.Point;
   import java.awt.Color;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;

   import com.sun.j3d.utils.universe.*;
   import com.sun.j3d.utils.geometry.*;
   import com.sun.j3d.utils.behaviors.mouse.*;
   import javax.media.j3d.*;
   import javax.vecmath.*;

   public class SKBaseConstraint3d implements SKItemInterface
   {
      public static SKMainFrame   frameMain;
   
      public int           ID;  //Unique Constraint ID
      public String        cType;
      public String        Name;
      public int           typeID;
      public SKShape3dArray  ShapeList = new SKShape3dArray(2);
      public int           drawMode;
      public boolean       isDrawn = true;
      public boolean       autoAddShapeProps = true;
      public boolean       isClone = false;
      public boolean       create = true;
      public SKBaseConstraint3d clonedConstraint = null;

      private static QuadCurve2D  curve = new QuadCurve2D.Float();
      public int           beServedBy;
   
      public SKBaseConstraint3d(SKMainFrame MainFrame, int idnum, String newname, String constraintType, int TypeID)
      {
         frameMain=MainFrame;
         ID=idnum;
         typeID = TypeID;
         Name=newname;
         cType=constraintType;
      }

      public TransformGroup getAddTrans()
      {
	 return null;
      } 
   
   //Severs all ties (for gc)
      public void severTies()
      {
         removeAllShapes();
         ShapeList = null;
         curve = null;
      }
      public SKBaseShape3d getOtherShape(SKBaseShape3d sh)
      {
         SKBaseShape3d s=ShapeList.get(1);
         for(int i=0; i<ShapeList.size(); i++)
            if(!(sh.equals(ShapeList.get(i))))
               s=ShapeList.get(i);
         return s;
      }
      public void updateMainProperties(SKPropArray vPropList)
      {
         frameMain.propFactory.freeProps(vPropList);
         vPropList.add(frameMain.propFactory.getNewProp("ID", Integer.toString(ID),0,false));
         vPropList.add(frameMain.propFactory.getNewProp("Type",cType,0,false));
         vPropList.add(frameMain.propFactory.getNewProp("Name",Name,0));
      
         if (autoAddShapeProps)
         { //Get properties for shapes
            for (int i=0; i<ShapeList.size(); i++)
            {
               //ShapeList.get(i).updateConstraintProps(this,vPropList);
            }
         }
      }
   
      public boolean setMainData(String PropName, Object PropData)
      {
         if (PropName=="Name")
         {
            Name = (String)PropData;
         }
         else
            return false;
      
         return true;
      }
   
   //Called by shape when adding props to contraint for info
      public int getConInfo(SKBaseShape3d sh, int code)
      {
         return 0;
      }
   
   //Called by primary shape, actually paints constraint lines
      public void doPaint()
      {
         if (!isDrawn && !(this instanceof SKAngleConstraint3d))
            return;
      
         SKBaseShape3d sh1 = ShapeList.get(0);
         SKBaseShape3d sh2 = ShapeList.get(1);
      
         Point3d pt1 = sh1.getConstraintPoint(this,drawMode),
         pt2 = sh2.getConstraintPoint(this,drawMode);
      
  	// System.out.println ( "Calling Draw constraint for.... " + sh1.ID + " " + sh2.ID );
         drawConstraint(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y,pt2.z);
      }
	
      public void doPaint(JSelectionPanel3d panel)
      {
         if (!isDrawn && !(this instanceof SKAngleConstraint3d))
            return;

         SKBaseShape3d sh1 = ShapeList.get(0);
         SKBaseShape3d sh2 = ShapeList.get(1);

         Point3d pt1 = sh1.getConstraintPoint(this,drawMode),
         pt2 = sh2.getConstraintPoint(this,drawMode);
       

         drawConstraint(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y,pt2.z, panel);
      }

   
      public void drawConstraint(double x1,double y1, double z1, double x2,double y2, double z2)
      {
      }

      public void drawConstraint(double x1,double y1, double z1, double x2,double y2, double z2, JSelectionPanel3d panel)
      {
      }
      public void changeColor(Color c)
      {
      }
      public void changeColor(Color3f c)
      {
      }

/*      public float distance(int x1, int y1, int x2, int y2)
      
      {
         return (float)java.lang.Math.sqrt(java.lang.Math.pow((double)(x1-x2),2)+java.lang.Math.pow((double)(y1-y2),2));
      }
   
      public int centerx(SKLineShape line)
      
      {
         if(line.pt1.getX() > line.pt2.getX())
            return (line.pt2.getX() + (line.pt1.getX()-line.pt2.getX())/2);
         else
            return (line.pt1.getX() + (line.pt2.getX()-line.pt1.getX())/2);
      }
   
   
      public int centery(SKLineShape line)
      
      
      {
         if(line.pt1.getY() > line.pt2.getY())
            return (line.pt2.getY() + (line.pt1.getY()-line.pt2.getY())/2);
         else
            return (line.pt1.getY() + (line.pt2.getY()-line.pt1.getY())/2);
      }
   
      public boolean lineintersection( double array[], double slope1, double slope2, double c1, double c2 )
      { //System.out.println(slope1+" "+slope2+" "+c1+" "+c2);
         array[0] = (c2 - c1) / (slope1 - slope2);
         array[1] = ( (slope1*c2) - (slope2*c1) ) / ( slope1 - slope2);
         if((slope1==0) || (slope2==0))
            return false;
         else
            return true;
      }
   
      public void drawExtension(Graphics g, SKLineShape line1, SKLineShape line2)
      
      {
         float[] dist=new float[4];
         double[] array=new double[2];
         double[] input=new double[7];
         double[] output=new double[2];
         float min;
         int num=0;
         SKPointShape pt=null,p=null;
         dist[0] = distance(line1.pt1.getX(),line1.pt1.getY(),line2.pt1.getX(),line2.pt1.getY());
         dist[1] = distance(line1.pt1.getX(),line1.pt1.getY(),line2.pt2.getX(),line2.pt2.getY());
         dist[2] = distance(line1.pt2.getX(),line1.pt1.getY(),line2.pt1.getX(),line2.pt1.getY());
         dist[3] = distance(line1.pt2.getX(),line1.pt1.getY(),line2.pt2.getX(),line2.pt2.getY());
         min = dist[0];
         for(int i=0; i<4; i++)
            if(dist[i]<min)
            {
               min = dist[i];
               num=i;
            }
         switch (num)
         {
            case 0: p = line1.pt1;
               pt = line2.pt1;
               break;
            case 1: p=line1.pt1;
               pt=line2.pt2;
               break;
            case 2 : p=line1.pt2;
               pt=line2.pt1;
               break;
            case 3 : p=line1.pt2;
               pt = line2.pt2;
               break;
         }
         int x=pt.getX(),y=pt.getY();
         double  slope=Double.MAX_VALUE, pslope=Double.MAX_VALUE;
         if(line1.pt2.getX()!= line1.pt1.getX())
         {   pslope=((double)(line1.pt2.getY()-line1.pt1.getY()))/((double)(line1.pt2.getX()-line1.pt1.getX()));}
         slope = -1/pslope;
         double c1,c2 ;
         c1= p.getY() -(pslope*p.getX());
         c2= pt.getY() -(slope*pt.getX());
         if(lineintersection(array,pslope,slope,c1,c2))
         {x=(int)array[0];
            y=(int)array[1];}
         else {
            if(slope ==0 )
               x=p.getX();
            else
               y=p.getY();
         }
         g.drawLine(pt.getX(),pt.getY(),(int)x,(int)y);
         if(! ( ((x<line1.pt1.getX() && x>line1.pt2.getX()) || (x>line1.pt1.getX() && x<line1.pt2.getX()))
               && ((x<line1.pt1.getY() && x>line1.pt2.getY()) || (x>line1.pt1.getY() && x<line1.pt2.getY())) ) )
            g.drawLine(p.getX(),p.getY(),(int)x,(int)y);
      
      }
   
      public void repaint()
      {
         frameMain.RefreshShapeArea( curve.getBounds() );
      }
*/
   
      public void addShape(SKBaseShape sh)
      {
         SKBaseShape3d s=null;
         for(int i=0; i<frameMain.allshapes3d.size(); i++)
            for(int j=0; j<=frameMain.allshapes3d.get(i).getNumSubShapes();j++)
               if(sh.ID == frameMain.allshapes3d.get(i).getSubShape(j).ID)
                  s = frameMain.allshapes3d.get(i).getSubShape(j);
         ShapeList.add(s);
         s.ConstraintList.add(this);
      }
   
      public void removeShape(SKBaseShape3d sh)
      {
         if (ShapeList.size()<3)
         {
            removeAllShapes();
         }
         else
         {
            ShapeList.removeShape(sh);
         
         //Remove from that shape's ConstraintList
            sh.ConstraintList.remove(this);
         }
      }
   
      public void removeAllShapes()
      {
         for (int i=0; i<ShapeList.size(); i++)
            ShapeList.get(i).ConstraintList.remove(this);
      
         ShapeList.clear();
         frameMain.DrawnItems.remove(this);
      }
   
      public boolean isPrimaryShape(SKBaseShape3d sh)
      {
         return (ShapeList.get(0)==sh);
      }
   
      public boolean doHitTest(int x, int y)
      {
         return curve.intersects(x-3,y-3,6,6);
      }
   
   //Makes sure this constraint is available with the given selectedShapes
      public static boolean isAvailable(SKShape3dArray selectedShapes)
      {
         return true;
      }
   
      public boolean constrEquals(SKShape3dArray selectedShapes, SKBaseConstraint3d con)
      {
         for (int i=0; i<selectedShapes.size(); i++)
            if (con.ShapeList.indexOf(selectedShapes.get(i)) == -1)
               return false;
      
         return true;
      }
   
      public String toString()
      {
         return Name;
      }
   
      public void writeToStream(DataOutputStream p, boolean writeID) throws IOException
      {
         p.writeInt(typeID);
         if (writeID)  p.writeInt(ID);
         p.writeUTF(Name);
      
         writeAdditionalProps(p);
      
      //Write associated ShapeIDs
         p.writeInt( ShapeList.size() );
         for (int i=0; i<ShapeList.size(); i++)
         {
            p.writeInt( ShapeList.get(i).ID );
         }
      }
   
      public void writeToStream(DataOutputStream p, boolean writeID, SKShapeArray shapes) throws IOException
      { //Make sure all associated shapes are in the shapes array
         if (shapes != null)
         {
            for (int i=0; i<ShapeList.size(); i++)
            {
               if ( shapes.indexOf( ShapeList.get(i) ) == -1 )
                  return;
            }
         }
      
         writeToStream(p,writeID);
      }
   
      public void writeAdditionalProps(DataOutputStream p) throws IOException
      {  }
   
      public void readFromStream(DataInputStream p) throws IOException
      {
         Name = p.readUTF();
      
         readAdditionalProps(p);
      }
   
      public void readAdditionalProps(DataInputStream p) throws IOException
      {  }
   
      public SKItemInterface getClonedItem()
      {
         return null;
      }
      public void cloneConstraint(SKBaseConstraint3d con, SKShape3dArray shapeArray)
      {
         con.Name = Name;
         con.isClone=true;
         con.clonedConstraint = this;
         con.ID = ID;
         for(int i=0;i<ShapeList.size(); i++)
         {
            SKBaseShape3d cloneShape = null;
            for(int j =0; j<shapeArray.size(); j++)
            {
               cloneShape = shapeArray.findByID(ShapeList.get(i).ID);
            }
            if(cloneShape!=null)
            {
               con.ShapeList.add(cloneShape);
               cloneShape.ConstraintList.add(con);
               //System.out.println("111 clone con = "+con+" shapeList = "+con.ShapeList.size()+" cloneShape = "+cloneShape+" conlist = "+cloneShape.ConstraintList.size());
            }
         }
      }
      /**************functions*********************/
      public static float magnitude(Vector3f v)
      {
         float answer = v.x*v.x + v.y*v.y + v.z*v.z;
         answer = (float)java.lang.Math.sqrt((double)answer);
         return answer;
      }
      public static void normalize(Vector3f v)
      {
         float mag = magnitude(v);
         v.x/=mag;
         v.y/=mag;
         v.z/=mag;
      }
      public static Vector3f crossProduct(Vector3f v1, Vector3f v2)
      {
         Vector3f answer = new Vector3f();
         answer.x = v1.y*v2.z - v1.z*v2.y;
         answer.y = v1.z*v2.x - v1.x*v2.z;
         answer.z = v1.x*v2.y - v1.y*v2.x;
         return answer;
      }
      public static float dotProduct(Vector3f v1, Vector3f v2) 
      {
         float answer;
         answer = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
         return answer;
      }
   }
