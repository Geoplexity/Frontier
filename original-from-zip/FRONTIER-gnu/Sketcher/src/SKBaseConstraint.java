/**
 * Title:        SKBaseConstraint
 * Description:  Base class for all constraints
 */
/*Copyright (C) June 22, 2001 Meera Sitharam

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
http://www.gnu.org/copyleft/gpl.html

You should have received a copy of the GNU General Public License
in the documentation index in the documentation
folder of the FRONTIER-gnu directory; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
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

   public class SKBaseConstraint implements SKItemInterface
   {
      public static SKMainFrame   frameMain;
   
      public int           ID;  //Unique Constraint ID
      public String        cType;
      public String        Name;
      public int           typeID;
      public SKShapeArray  ShapeList = new SKShapeArray(2);
      public int           drawMode;
      public boolean       isDrawn = true;
      public boolean       autoAddShapeProps = true;
      private static QuadCurve2D  curve = new QuadCurve2D.Float();
      public int           beServedBy;
   
      public SKBaseConstraint(SKMainFrame MainFrame, int idnum, String newname, String constraintType, int TypeID)
      {
         frameMain=MainFrame;
         ID=idnum;
         typeID = TypeID;
         Name=newname;
         cType=constraintType;
      }
   
   //Severs all ties (for gc)
      public void severTies()
      {
         removeAllShapes();
         ShapeList = null;
         curve = null;
      }
      public SKBaseShape getOtherShape(SKBaseShape sh)
      {
         SKBaseShape s=ShapeList.get(1);
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
               ShapeList.get(i).updateConstraintProps(this,vPropList);
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
      public int getConInfo(SKBaseShape sh, int code)
      {
         return 0;
      }
   
   //Called by primary shape, actually paints constraint lines
      public void doPaint(Graphics g)
      {
         if (!isDrawn && !(this instanceof SKParallelConstraint) &&!(this instanceof SKPerpConstraint) && !(this instanceof SKAngleConstraint)) 
            return;
      
         SKBaseShape sh1 = ShapeList.get(0);
         SKBaseShape sh2 = ShapeList.get(1);
      
         Point pt1 = sh1.getConstraintPoint(this,drawMode),
         pt2 = sh2.getConstraintPoint(this,drawMode);
      
      
         //if(this instanceof SKPerpConstraint && solved)
           // drawConstraint(g,pt1.x,pt1.y,pt2.x,pt2.y);
         drawConstraint(g,pt1.x,pt1.y,pt2.x,pt2.y);
      }
   
      public void drawConstraint(Graphics g,int x1,int y1,int x2,int y2)
      {//g.drawLine(x1,y1,x2,y2);
      //g.drawArc(x1,java.lang.Math.min(y1,y2)-20,java.lang.Math.abs(x2-x1),java.lang.Math.abs(y1-y2)+20,5,360);
         if (!frameMain.editingShape && this==frameMain.SelectedConstraint)
            g.setColor(Color.red);
         else  if (this==frameMain.mouseOverItem)
            g.setColor(Color.white);
         else
            g.setColor(Color.black);
      
         if(this instanceof SKDistanceConstraint)
         { //Draw line between first and second shape
            boolean arbit1=true, arbit2=true;
            if (!frameMain.editingShape && this==frameMain.SelectedConstraint)
               g.setColor(Color.red);
            else  if (this==frameMain.mouseOverItem)
               g.setColor(Color.white);
            else
               g.setColor(Color.black);
            if( (((SKDistanceConstraint)this).perp))
            {
               for(int i=0; i<ShapeList.size(); i++)
                  for(int j=0; j<ShapeList.get(i).getNumSubShapes(); j++)
                  {
                     SKBaseShape s = ShapeList.get(i).getSubShape(j);
                     arbit1 = arbit1 && ( !((s.getShapeX() == x1) && (s.getShapeY() == y1)) );
                     arbit2 = arbit2 && ( !((s.getShapeX() == x2) && (s.getShapeY() == y2)) );
                  }
               if( arbit1 )
               {
                  SKBaseShape sh1 = ShapeList.get(0);
                  SKBaseShape sh2 = ShapeList.get(1);
                  double slope1 = (double)(((SKLineShape)sh1).pt2.getShapeY() - ((SKLineShape)sh1).pt1.getShapeY()) / (((SKLineShape)sh1).pt2.getShapeX() - ((SKLineShape)sh1).pt1.getShapeX());
                  double c1 = ((SKLineShape)sh1).pt2.getShapeY()  -(slope1*((SKLineShape)sh1).pt2.getShapeX());
                  double slope2 = -1/slope1;
                  double c2 = (y2  -(slope2*x2));
                  double[] array = new double[4];
                  //System.out.println("slope "+slope1+" "+((SKLineShape)sh1).pt2.getShapeY()+" "+((SKLineShape)sh1).pt1.getShapeY()+" "+((SKLineShape)sh1).pt2.getShapeX()+" "+((SKLineShape)sh1).pt1.getShapeX());
                  if(lineintersection(array,slope1,slope2,c1,c2))
                  {
                     x1=(int)array[0];
                     y1=(int)array[1];}
                  else {
                     if(slope1 ==0 )
                        x1=x2;
                     else if(slope2 ==0)
                        y1=y2;	
                  }
               }
               else if ( arbit2 )
               {
                  SKBaseShape sh1 = ShapeList.get(0);
                  SKBaseShape sh2 = ShapeList.get(1);
                  double slope1 = (double)(((SKLineShape)sh2).pt2.getShapeY() - ((SKLineShape)sh2).pt1.getShapeY()) / (((SKLineShape)sh2).pt2.getShapeX() - ((SKLineShape)sh2).pt1.getShapeX());
                  double c1 = ((SKLineShape)sh2).pt2.getShapeY()  -(slope1*((SKLineShape)sh2).pt2.getShapeX());
                  double slope2 = -1/slope1;
                  double c2 = (y1  -(slope2*x1));
                  double[] array = new double[4];
                  //System.out.println("slope "+slope1);
                  if(lineintersection(array,slope1,slope2,c1,c2))
                  {
                     x2=(int)array[0];
                     y2=(int)array[1];}
                  else {
                     if(slope1 ==0 )
                        x2=x1;
                     else
                        y2=y1;	
                  }
               
               }
            
            }
            g.drawLine(x1,y1,x2,y2);
         }
         if(this instanceof SKPerpConstraint)
         {
            SKLineShape line1=(SKLineShape)ShapeList.get(0), line2=(SKLineShape)ShapeList.get(1);
            SKPointShape closePoint1=null, closePoint2=null;
            int centerx1=centerx(line1), centery1=centery(line1), centerx2=centerx(line2), centery2=centery(line2);
            drawExtension(g,line1,line2);
         }
         if(this instanceof SKParallelConstraint)
         {
            SKLineShape line1 = (SKLineShape) ShapeList.get(0);
            SKLineShape line2 = (SKLineShape) ShapeList.get(1);
            int centerx1, centery1, centerx2, centery2, x11, x12, x21, x22, y11, y12, y21, y22;
            float slope = Float.MAX_VALUE,dslope ; 
            if(line1.pt1.getX() != line1.pt2.getX())
               slope = (line1.pt1.getY()-line1.pt2.getY()) / (line1.pt1.getX()-line1.pt2.getX()); 
            dslope = -1/slope;
            SKBaseConstraint cons1 =  null, cons2 = null;
            for(int i=0; i<line1.ConstraintList.size(); i++)
            {cons1 = line1.ConstraintList.get(i);
               if(cons1 instanceof SKParallelConstraint)
                  ((SKParallelConstraint)this).count=((SKParallelConstraint)cons1).count;
            }
            if(((SKParallelConstraint)this).count!=frameMain.paracount)frameMain.paracount++;
            if(line1.pt1.getX()>line1.pt1.getX()) 
               centerx1 = (int) (line1.pt2.getShapeX() + ((line1.pt1.getShapeX()-line1.pt2.getShapeX())/2));
            else  
               centerx1 = (int) (line1.pt1.getShapeX() + ((line1.pt2.getShapeX()-line1.pt1.getShapeX())/2));
            if(line2.pt1.getX()>line2.pt1.getX()) 
               centerx2 = (int) (line2.pt2.getShapeX() + ((line2.pt1.getShapeX()-line2.pt2.getShapeX())/2));
            else  
               centerx2 = (int) (line2.pt1.getShapeX() + ((line2.pt2.getShapeX()-line2.pt1.getShapeX())/2));
            if(line1.pt1.getY()>line1.pt1.getY()) 
               centery1 = (int) (line1.pt2.getShapeY() + ((line1.pt1.getShapeY()-line1.pt2.getShapeY())/2));
            else  
               centery1 = (int) (line1.pt1.getShapeY() + ((line1.pt2.getShapeY()-line1.pt1.getShapeY())/2));
            if(line2.pt1.getY()>line2.pt1.getY()) 
               centery2 = (int) (line2.pt2.getShapeY() + ((line2.pt1.getShapeY()-line2.pt2.getShapeY())/2));
            else  
               centery2 = (int) (line2.pt1.getShapeY() + ((line2.pt2.getShapeY()-line2.pt1.getShapeY())/2));
            x11 = centerx1;
            x12 = centerx1;
            x21 = centerx2;
            x22 = centerx2;
         
            for(int i=0; i<((SKParallelConstraint)this).count ; i++)
            {
               x11=x11+2;
               x12=x12+2;
               x21=x21+2;
               x22=x22+2;
               if(slope!=0)
               {
                  y11 = centery1 + ((int)dslope*(x11-centerx1));
                  y12 = centery1 + ((int)dslope*(x12-centerx1));
                  y21 = centery2 + ((int)dslope*(x21-centerx2));
                  y22 = centery2 + ((int)dslope*(x22-centerx2));}
               else
               {
                  y11=centery1+2;
                  y12=centery1-2;
                  y21=centery2+2;
                  y22=centery2-2;}
               if (frameMain.panelShapeArea.DrawConstraints && frameMain.drawConstrDetails)
               {
                  g.drawLine(x11,y11,x12,y12);
                  g.drawLine(x21,y21,x22,y22);
               
               }
            }}
      
         if(this instanceof SKAngleConstraint)
         {  
            SKLineShape line1 = (SKLineShape) ShapeList.get(0);
            SKLineShape line2 = (SKLineShape) ShapeList.get(1);
            double[] array=new double[2];
            double min;
            int num=0;
            SKPointShape pt=line2.pt1,p=line1.pt1;
            int x,y;
            double  slope1=Double.MAX_VALUE, slope2=Double.MAX_VALUE;
            if(line1.pt2.getShapeX()!= line1.pt1.getShapeX())
            {   slope1=((double)(line1.pt2.getShapeY()-line1.pt1.getShapeY()))/((double)(line1.pt2.getShapeX()-line1.pt1.getShapeX()));}
           /* if(line1.getLeftPoint().getShapeY()<line2.getLeftPoint().getShapeY())
               slope2 = java.lang.Math.tan(java.lang.Math.PI/180* ((java.lang.Math.atan(slope1))+180+(((SKAngleConstraint)this).angle)));
            else 
               slope2 = java.lang.Math.tan (java.lang.Math.PI/180*(java.lang.Math.atan(slope1)+((SKAngleConstraint)this).angle));*/
            slope2 = java.lang.Math.tan (java.lang.Math.atan(slope1)+java.lang.Math.toRadians(((SKAngleConstraint)this).angle));
            double c1,c2 ;
            c1= p.getShapeY() -(slope1*p.getShapeX());
            c2= pt.getShapeY() -(slope2*pt.getShapeX());
            lineintersection(array,slope1,slope2,c1,c2);
            x=(int)array[0];
            y=(int)array[1];
            double dist1 = distance(x,y,line1.pt1.getShapeX(),line1.pt1.getShapeY());
            double dist2 = distance(x,y,line1.pt2.getShapeX(),line1.pt2.getShapeY());
            if(dist1>dist2)
               p=line1.pt2;
            dist1 = distance(x,y,line2.pt1.getShapeX(),line2.pt1.getShapeY());
            dist2 = distance(x,y,line2.pt2.getShapeX(),line2.pt2.getShapeY());
            if(dist1>dist2)
               pt=line2.pt2;
            g.drawLine(pt.getShapeX(),pt.getShapeY(),(int)x,(int)y);
            g.drawLine(p.getShapeX(),p.getShapeY(),(int)x,(int)y);
            g.drawArc(x-3,y-3,3,3,(int)java.lang.Math.atan(slope1),(int)java.lang.Math.atan(slope2));
         }
      
      /*
      //Calculate starting point for all other lines
      x1 = java.lang.Math.min(x1,x2)+(java.lang.Math.abs(x1-x2) >> 1);
      y1 = java.lang.Math.min(y1,y2)+(java.lang.Math.abs(y1-y2) >> 1);
      
      //Draw lines from x1,y1 to all other shapes
      for (int i=2; i<ShapeList.size(); i++)
      {
      sh2 = ShapeList.get(i);
      g.drawLine(x1,y1,(sh2.getWidth() >> 1)+sh2.getX(),(sh2.getHeight() >> 1)+sh2.getY());
      }
      */
      }
   
      public float distance(int x1, int y1, int x2, int y2)
      
      {
         return (float)java.lang.Math.sqrt(java.lang.Math.pow((double)(x1-x2),2)+java.lang.Math.pow((double)(y1-y2),2));
      }
   
      public int centerx(SKLineShape line)
      
      {
         if(line.pt1.getShapeX() > line.pt2.getShapeX())
            return (line.pt2.getShapeX() + (line.pt1.getShapeX()-line.pt2.getShapeX())/2);
         else 
            return (line.pt1.getShapeX() + (line.pt2.getShapeX()-line.pt1.getShapeX())/2);
      }
   
   
      public int centery(SKLineShape line)
      
      
      {
         if(line.pt1.getShapeY() > line.pt2.getShapeY())
            return (line.pt2.getShapeY() + (line.pt1.getShapeY()-line.pt2.getShapeY())/2);
         else 
            return (line.pt1.getShapeY() + (line.pt2.getShapeY()-line.pt1.getShapeY())/2);
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
         dist[0] = distance(line1.pt1.getShapeX(),line1.pt1.getShapeY(),line2.pt1.getShapeX(),line2.pt1.getShapeY());
         dist[1] = distance(line1.pt1.getShapeX(),line1.pt1.getShapeY(),line2.pt2.getShapeX(),line2.pt2.getShapeY());
         dist[2] = distance(line1.pt2.getShapeX(),line1.pt1.getShapeY(),line2.pt1.getShapeX(),line2.pt1.getShapeY());
         dist[3] = distance(line1.pt2.getShapeX(),line1.pt1.getShapeY(),line2.pt2.getShapeX(),line2.pt2.getShapeY());
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
         int x=pt.getShapeX(),y=pt.getShapeY();
         double  slope=Double.MAX_VALUE, pslope=Double.MAX_VALUE;
         if(line1.pt2.getShapeX()!= line1.pt1.getShapeX())
         {   pslope=((double)(line1.pt2.getShapeY()-line1.pt1.getShapeY()))/((double)(line1.pt2.getShapeX()-line1.pt1.getShapeX()));}
         //if(line2.pt2.getShapeX()!= line2.pt1.getShapeX())
         //{   slope=((double)(line2.pt2.getShapeY()-line2.pt1.getShapeY()))/((double)(line2.pt2.getShapeX()-line2.pt1.getShapeX()));}
         slope = -1/pslope;
         double c1,c2 ;
         c1= p.getShapeY() -(pslope*p.getShapeX());
         c2= pt.getShapeY() -(slope*pt.getShapeX());
         if(lineintersection(array,pslope,slope,c1,c2))
         {x=(int)array[0];
            y=(int)array[1];}
         else {
            if(slope ==0 )
               x=p.getShapeX();
            else
               y=p.getShapeY();	
         }
         //System.out.println("x "+x+" y "+y+" p "+p+" pt "+pt);
         g.drawLine(pt.getShapeX(),pt.getShapeY(),(int)x,(int)y);
         if(! ( ((x<line1.pt1.getX() && x>line1.pt2.getX()) || (x>line1.pt1.getX() && x<line1.pt2.getX())) 
               && ((x<line1.pt1.getY() && x>line1.pt2.getY()) || (x>line1.pt1.getY() && x<line1.pt2.getY())) ) )
            g.drawLine(p.getShapeX(),p.getShapeY(),(int)x,(int)y);
      
      }
   
      public void repaint()
      {
         frameMain.RefreshShapeArea( curve.getBounds() );
      }
   
      public void addShape(SKBaseShape sh)
      {
         ShapeList.add(sh);
      
      //Add to Shape's ConstraintList
         sh.ConstraintList.add(this);
      }
   
      public void removeShape(SKBaseShape sh)
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
   
      public boolean isPrimaryShape(SKBaseShape sh)
      {
         return (ShapeList.get(0)==sh);
      }
   
      public boolean doHitTest(int x, int y)
      {
         return curve.intersects(x-3,y-3,6,6);
      }
   
   //Makes sure this constraint is available with the given selectedShapes
      public static boolean isAvailable(SKShapeArray selectedShapes)
      {
         return true;
      }
   
      public boolean constrEquals(SKShapeArray selectedShapes, SKBaseConstraint con)
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
   }
