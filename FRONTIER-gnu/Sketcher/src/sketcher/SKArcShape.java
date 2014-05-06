/**
 * Title:        SKArcShape
 * Description:  Represents an arc with 1 center point and two end points
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Graphics2D;
   import java.awt.Color;
   import java.awt.Point;
   import java.awt.geom.Arc2D;
   import java.awt.Stroke;
   import java.awt.BasicStroke;
   import java.awt.Color;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;

   public class SKArcShape extends SKBaseShape
   {
      public SKPointSubShape center = null;
      public SKPointSubShape pt1 = null;
      public SKPointSubShape pt2 = null;
      public int radius = 40;
      public int orientation = 0;
      public double angle = 60,newangle1=0, newangle2=0, angle1=0, angle2=0;
      public boolean bifur=false,fixangle=false, fixradius=false, Adjust=false;
      public int centerx,centery,pt1x,pt1y,pt2x,pt2y;
   
      public static final String[] ConType = new String[] {"Center","Point 1","Point 2","Circumferrence"};
      public Arc2D.Float arcShape;
      public static SKMainFrame frameMain;
      public SKArcShape(SKMainFrame frame, int newID, int x, int y)
      {
         super(frame,newID,"Arc"+newID,"Arc",5);
      
      //Create points
         center = new SKPointSubShape(frame,newID+1,x,y,this);
         pt1 = new SKPointSubShape(frame, newID+2, (int)(x + (radius * (java.lang.Math.cos(java.lang.Math.toRadians(orientation-(angle/2)))))), (int)(y - (radius * (java.lang.Math.sin(java.lang.Math.toRadians(orientation-(angle/2)))))), this);
         pt2 = new SKPointSubShape(frame, newID+3, (int)(x + (radius * (java.lang.Math.cos(java.lang.Math.toRadians(orientation+(angle/2)))))), (int)(y - (radius * (java.lang.Math.sin(java.lang.Math.toRadians(orientation+(angle/2)))))), this);
         arcShape = new Arc2D.Float(center.getX()+(center.getWidth()/2)-radius,center.getY()+(center.getHeight()/2)-radius,2*radius,2*radius,-(int)(angle2*180/3.141),-(int)((angle1-angle2)*180/3.141),Arc2D.OPEN);
      
         frameMain=frame;
         IconPath = "/images/arcicon.gif";
         centerx=center.getX();
         centery=center.getY();
         pt1x=pt1.getX();
         pt1y=pt1.getY();
         pt2x=pt2.getX();
         pt2y=pt2.getY();
      }
      public int getX()
      {
         return center.getX();}
      public int getY()
      {
         return center.getY();}
   
      public void severTies()
      {
         super.severTies();
         center = null;
         pt1 = null;
         pt2 = null;
      }
      public SKBaseShape getSubShape(int index)
      {
         switch (index)
         {
            case 1 : 
               return center;
            case 2 : 
               return pt1;
            case 3 :
               return pt2;
            default: 
               return this;
         }
      }
   
      public void updateMainShapeData(SKPropArray vPropList, SKConstraintArray vCurrentConstraints)
      {
         //super.updateMainShapeData(vPropList,vCurrentConstraints);
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
         if (vPropList != null)
         { //Update properties
            vPropList.add(frameMain.propFactory.getNewProp("Radius", Integer.toString(radius),1,Color.red));
            vPropList.add(frameMain.propFactory.getNewProp("Orientation", Integer.toString(orientation),1,Color.red));
            vPropList.add(frameMain.propFactory.getNewProp("Angle", Integer.toString((int)angle),1,Color.red));
            vPropList.add(frameMain.propFactory.getNewProp("Center X", Integer.toString(center.getShapeX()),1) );
            vPropList.add(frameMain.propFactory.getNewProp("Center Y", Integer.toString(center.getShapeY()),1) );
         }
      }
   
      public boolean setShapeData(String PropName, Object PropData)
      {
         if (super.setShapeData(PropName,PropData)) 
            return true;
      
         if (PropName=="Radius")
         {//setSize(getWidth(),new Integer(PropData.toString()).intValue());
            radius = new Integer(PropData.toString()).intValue();
         }
         else if (PropName=="Orientation")
         {
            orientation = new Integer(PropData.toString()).intValue();
            orientation = orientation;
         }
         else if (PropName=="Angle")
         {
            angle = new Integer(PropData.toString()).intValue();
            angle = angle;
         }
         else if (PropName=="Center X")
         {
            center.setLocation(new Integer(PropData.toString()).intValue()-(center.getWidth()>>1),center.getY());
         }
         else if (PropName=="Center Y")
         {
            center.setLocation(center.getX(),new Integer(PropData.toString()).intValue()-(center.getHeight()>>1));
         }
         else 
            return false;
      
      //Make panelShapeArea repaint
         frameMain.RefreshShapeArea();
      
         return true;
      }
      public void updateColors()
      {
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
         else
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.shapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.shapeOutlineColor];
         }
      }
   
   //Should this shape be drawn?
      public boolean shouldDraw()
      {   
         return (!frameMain.drawGroupRects || frameMain.selectedGroupShapes.indexOf(this) > -1);
      }
   
      public void paintSpecial(Graphics g)
      {
         updateColors();
         g.setColor(outlineColor);
	 super.paintSpecial(g);
         if (!fixradius && !fixangle)
         {         
            double dist1=java.lang.Math.sqrt(java.lang.Math.pow((double)(centerx-pt1x),2)+java.lang.Math.pow((double)(centery-pt1y),2));
            double height1=(double)(pt1y-centery);
            angle1=java.lang.Math.asin(height1/dist1);
            double dist2=java.lang.Math.sqrt(java.lang.Math.pow((double)(centerx-pt2x),2)+java.lang.Math.pow((double)(centery-pt2y),2));
            double height2=(double)(pt2y-centery);
            angle2=java.lang.Math.asin(height2/dist2);  
            g.drawArc(center.getX()+(center.getWidth()/2)-radius,center.getY()+(center.getHeight()/2)-radius,2*radius,2*radius,-(int)(angle2*180/3.141),-(int)((angle1-angle2)*180/3.141));
            arcShape.setArc(center.getX()+(center.getWidth()/2)-radius,center.getY()+(center.getHeight()/2)-radius,2*radius,2*radius,-(int)(angle2*180/3.141),-(int)((angle1-angle2)*180/3.141),Arc2D.OPEN);
         }
         Graphics2D g2d = (Graphics2D) g;
         g2d.setColor(Color.black);
         g2d.setStroke( new BasicStroke(1,BasicStroke.CAP_BUTT,BasicStroke.JOIN_BEVEL,0,new float[] {3,3},0) );
         g2d.drawLine(center.getX()+center.getWidth()/2,center.getY()+center.getHeight()/2,pt1.getX()+pt1.getWidth()/2,pt1.getY()+pt1.getHeight()/2);
         g2d.drawLine(center.getX()+center.getWidth()/2,center.getY()+center.getHeight()/2,pt2.getX()+pt2.getWidth()/2,pt2.getY()+pt2.getHeight()/2);
         g2d.setStroke( new BasicStroke(1) );
         if( Adjust)
         {
            if(fixradius)
            {
               g.setColor(outlineColor);
               g.drawArc(center.getX()+(center.getWidth()/2)-radius,center.getY()+(center.getHeight()/2)-radius,2*radius,2*radius,-(int)(newangle2*180/3.141),-(int)((newangle1-newangle2)*180/3.141));
               arcShape.setArc(center.getX()+(center.getWidth()/2)-radius,center.getY()+(center.getHeight()/2)-radius,2*radius,2*radius,-(int)(newangle2*180/3.141),-(int)((newangle1-newangle2)*180/3.141),Arc2D.OPEN);
            }
            if(fixangle)
            {
               g.setColor(outlineColor);
               g.drawArc(center.getX()+(center.getWidth()/2)-radius,center.getY()+(center.getHeight()/2)-radius,2*radius,2*radius,-(int)(angle2*180/3.141),-(int)((angle1-angle2)*180/3.141));
               arcShape.setArc(center.getX()+(center.getWidth()/2)-radius,center.getY()+(center.getHeight()/2)-radius,2*radius,2*radius,-(int)(angle2*180/3.141),-(int)((angle1-angle2)*180/3.141),Arc2D.OPEN);
            }
         }
      }
      public void fixRadiusAdjust()
      {
         double newdist1=java.lang.Math.sqrt(java.lang.Math.pow((double)(centerx-pt1.getX()),2)+java.lang.Math.pow((double)(centery-pt1.getY()),2));
         double newheight1=(double)(pt1.getY()-centery);
         newangle1=java.lang.Math.asin(newheight1/newdist1);
         double newdist2=java.lang.Math.sqrt(java.lang.Math.pow((double)(centerx-pt2.getX()),2)+java.lang.Math.pow((double)(centery-pt2.getY()),2));
         double newheight2=(double)(pt2.getY()-centery);
         newangle2=java.lang.Math.asin(newheight2/newdist2);
         if((pt1x!=pt1.getX() || pt1y!=pt1.getY()) ||  (pt2x!=pt2.getX() || pt2y!=pt2.getY()))
         {
            double disto=java.lang.Math.sqrt(java.lang.Math.pow((double)(centerx-pt1x),2)+java.lang.Math.pow((double)(centery-pt1y),2));
            pt1.setLocation((int)(centerx+(disto*java.lang.Math.cos(newangle1))),(int)(centery+(disto*java.lang.Math.sin(newangle1))));
            pt2.setLocation((int)(centerx+(disto*java.lang.Math.cos(newangle2))),(int)(centery+(disto*java.lang.Math.sin(newangle2))));
         }
         centerx=center.getX();
         centery=center.getY();
         pt1x=pt1.getX();
         pt1y=pt1.getY();
         pt2x=pt2.getX();
         pt2y=pt2.getY();
      }
   
      public void fixAngleAdjust()
      {
         double dist1=java.lang.Math.sqrt(java.lang.Math.pow((double)(centerx-pt1x),2)+java.lang.Math.pow((double)(centery-pt1y),2));
         double height1=(double)(pt1y-centery);
         angle1=java.lang.Math.asin(height1/dist1);
         double dist2=java.lang.Math.sqrt(java.lang.Math.pow((double)(centerx-pt2x),2)+java.lang.Math.pow((double)(centery-pt2y),2));
         double height2=(double)(pt2y-centery);
         angle2=java.lang.Math.asin(height2/dist2); 
         if(pt1.getX()!=pt1x || pt1.getY()!=pt1y)
         {double newdist1=java.lang.Math.sqrt(java.lang.Math.pow((double)(centerx-pt1.getX()),2)+java.lang.Math.pow((double)(centery-pt1.getY()),2));
            pt1.setLocation((int)(centerx+(newdist1*java.lang.Math.cos(angle1))),(int)(centery+(newdist1*java.lang.Math.sin(angle1))));
            pt2.setLocation((int)(centerx+(newdist1*java.lang.Math.cos(angle2))),(int)(centery+(newdist1*java.lang.Math.sin(angle2))));
            radius=(int)newdist1;}
         else if(pt2.getX()!=pt2x || pt2.getY()!=pt2y)
         {double newdist2=java.lang.Math.sqrt(java.lang.Math.pow((double)(centerx-pt2.getX()),2)+java.lang.Math.pow((double)(centery-pt2.getY()),2));
            pt1.setLocation((int)(centerx+(newdist2*java.lang.Math.cos(angle1))),(int)(centery+(newdist2*java.lang.Math.sin(angle1))));
            pt2.setLocation((int)(centerx+(newdist2*java.lang.Math.cos(angle2))),(int)(centery+(newdist2*java.lang.Math.sin(angle2))));
            radius=(int)newdist2;}
         centerx=center.getX();
         centery=center.getY();
         pt1x=pt1.getX();
         pt1y=pt1.getY();
         pt2x=pt2.getX();
         pt2y=pt2.getY();
      }
   
      public void setSelected(boolean sel)
      {
         if (sel != Selected)
         {
            Selected = sel;
            center.setSelected(sel);
            pt1.setSelected(sel);
            pt2.setSelected(sel);
         }
      }
   
      public void doMove(int deltaX, int deltaY, boolean resolveConstraints)
      {
         center.doMove(deltaX,deltaY,false);
         pt1.doMove(deltaX,deltaY,false);
         pt2.doMove(deltaX,deltaY,resolveConstraints);
      }
   
      public boolean doHitTest(int x, int y)
      {
         return arcShape.intersects(x-3,y-3,6,6);
      }
   
      public Point getConstraintPoint(SKBaseConstraint con, int mod)
      {
         Point pt = new Point();
      
         switch (mod)
         {
            case  0:  
               { //Center
                  pt.x=center.getShapeX();
                  pt.y=center.getShapeY();
               }
               break;
            case  1:  
               { //Circumferrence
                  pt.x=center.getShapeX()+radius;
                  pt.y=center.getShapeY();
               }
               break;
            default:  
               { //Arbitrary
                  pt.x=center.getShapeX()+radius>>1;
                  pt.y=center.getShapeY();
               }
         }
         return pt;
      }
   
      public Point getConstraintPoint(SKBaseConstraint con)
      {
         return new Point(center.getShapeX(),center.getShapeY());
      }
   
      public int getConstraintMod(SKBaseShape sh)
      {
         if (sh == center)
            return 1;
         else
            return 0;
      }
   
      public int getNumSubShapes()
      {
         return 3;
      }
   
      public void updateConstraintProps(SKBaseConstraint con, SKPropArray vPropList)
      {
         super.updateConstraintProps(con,vPropList);
      
         vPropList.add( frameMain.propFactory.getNewComboProp(Name,con.getConInfo(this,0),1,ConType) );
      }
   
      public int getConPropInfo(SKBaseConstraint con, String PropName, Object PropData)
      {
         if (PropData=="Center")
            return 0;
         else
            return 1;
      }
   
      public void repaint()
      {
         frameMain.RefreshShapeArea( arcShape.getBounds() );
      }
   
      public Point getPointForDistance(SKBaseShape otherShape)
      {
         return new Point(center.getShapeX(),center.getShapeY());
      }
   
      public void writeAdditionalProps(DataOutputStream p) throws IOException
      {
         super.writeAdditionalProps(p);
      
         center.writeAdditionalProps(p);
      }
   
      public void readAdditionalProps(DataInputStream p) throws IOException
      {
         super.readAdditionalProps(p);
      
         center.readAdditionalProps(p);
      }
      public void readAdditionalProps(DataInputStream p, int x, int y) throws IOException
      
      {
         super.readAdditionalProps(p);
      
         center.readAdditionalProps(p,x,y);
      }
   
      public boolean isPrimaryShape(SKBaseShape sh)
      {
         return (center==sh);
      }
      public void setShapePoint(Point pt)
      {
         int x1 = center.getShapeX();
         int y1 = center.getShapeY();
         int x2 = pt1.getShapeX();
         int y2 = pt1.getShapeY();
         int x3 = pt2.getShapeX();
         int y3 = pt2.getShapeY();
         int diffx = this.getShapeX()-pt.x;
         int diffy = this.getShapeY()-pt.y;
         center.setShapePoint(new Point(x1-diffx,y1-diffy));
         pt1.setShapePoint(new Point(x2-diffx, y2-diffy));
         pt2.setShapePoint(new Point(x3-diffx, y3-diffy));
      }
   }
