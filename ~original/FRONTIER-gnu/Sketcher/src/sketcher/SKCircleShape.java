/**
 * Title:        SKCircleShape
 * Description:  Represents a circle with 1 center point
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Color;
   import java.awt.Point;
   import java.awt.geom.Ellipse2D;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;

   public class SKCircleShape extends SKBaseShape
   {
      public SKPointSubShape center;
      public SKBaseShape selShape = this;
      public double radius = -1;
      public static SKMainFrame frameMain;
      public boolean bifur=false;
      public static final String[] ConType = new String[] {"Arbitrary","Center","Circumference"};
      public Ellipse2D.Float ellipseShape;
      public float scale=1;
   
      public SKCircleShape(SKMainFrame frame, int newID, int x, int y)
      {
         super(frame,newID,"Circle"+newID,"Circle",4);
         center = new SKPointSubShape(frame,newID+1,x,y,this);
         ellipseShape = new Ellipse2D.Float(x-15,y-15,30,30);
         IconPath = "/images/circleicon.gif";
         frameMain=frame;
         scale = frameMain.lastScale;
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
         {bodyColor    = SKOptions.colorOptions[SKOptions.moShapeColor];
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
   
   
      public void severTies()
      {
         super.severTies();
        // center.severTies();
         center = null;
      }
   
      public int getX()
      {
         return center.getX();}
      public int getY()
      {
         return center.getY();}
   
   
      public void updateMainShapeData(SKPropArray vPropList, SKConstraintArray vCurrentConstraints)
      {
        // super.updateMainShapeData(vPropList,vCurrentConstraints);
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
            vPropList.add(frameMain.propFactory.getNewProp("Radius", Double.toString(radius),1,Color.red));
            vPropList.add(frameMain.propFactory.getNewProp("Center X", Integer.toString(center.getShapeX()),1) );
            vPropList.add(frameMain.propFactory.getNewProp("Center Y", Integer.toString(center.getShapeY()),1) );
         }
      }
   
      public boolean setShapeData(String PropName, Object PropData)
      {
         if (super.setShapeData(PropName,PropData)) 
            return true;
      
         if (PropName=="Radius")
         {
         //setSize(getWidth(),new Integer(PropData.toString()).intValue());
            radius = new Double(PropData.toString()).doubleValue();
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
   
      public void paintSpecial(Graphics g)
      {
	 updateColors();
	 g.setColor(outlineColor);
         super.paintSpecial(g);
         int r=(int)(radius*frameMain.lastScale*80);
         if(!bifur )
	 { 
	 g.drawOval(center.getX()+(center.getWidth()/2)-(int)(20*frameMain.lastScale),center.getY()+(center.getHeight()/2)-(int)(20*frameMain.lastScale),(int)(40*frameMain.lastScale),(int)(40*frameMain.lastScale));
	 ellipseShape.setFrame(center.getX()+(center.getWidth()/2)-(int)(20*frameMain.lastScale),center.getY()+(center.getHeight()/2)-(int)(20*frameMain.lastScale),(int)(40*frameMain.lastScale),(int)(40*frameMain.lastScale));
	 }
         else
	 { 
	 g.drawOval(center.getX()+(center.getWidth()/2)-(int)(r/2),center.getY()+(center.getHeight()/2)-(int)(r/2),r,r);
	 ellipseShape.setFrame(center.getX()+(center.getWidth()/2)-(int)(r/2),center.getY()+(center.getHeight()/2)-(int)(r/2),r,r);
	 }

      }
   
      public void setSelected(boolean sel)
      {
         if (sel != Selected)
         {
            Selected = sel;
            center.setSelected(sel);
         }
      }
   
      public void doMove(int deltaX, int deltaY, boolean resolveConstraints)
      {
         center.doMove(deltaX,deltaY,resolveConstraints);
         repaint();
      }
      public boolean doHitTest(int x, int y)
      {
         return ellipseShape.intersects(x-3,y-3,6,6);
      }
      public void increaseRadius( int x, int y)
      {
	bifur = true;
         radius = distance(center.getShapeX(), center.getShapeY(), x, y);
         radius = radius/(40*frameMain.lastScale);
         repaint();
      }
      public void increaseRadius(double rad)
      {bifur = true;
         radius = rad/(40*frameMain.lastScale);
         repaint();
      }
      public Point getConstraintPoint(SKBaseConstraint con, int mod)
      {
         Point pt = new Point();
      
         switch (mod)
         {
            case  1:  
               { //Center
                  pt.x=center.getShapeX();
                  pt.y=center.getShapeY();
               }
               break;
            case  2:  
               { //Circumferrence
                  if(radius==-1)pt.x=center.getShapeX()+20;
                  else pt.x=center.getShapeX()+(int)radius;
                  pt.y=center.getShapeY();
               }
               break;
            default:  
               { //Arbitrary
                  pt.x=center.getShapeX()+20>>1;
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
         return 1;
      }
   
      public SKBaseShape getSubShape(int index)
      {
         switch (index)
         {
            case 1 : 
               return center;
            default: 
               return this;
         }
      }
   
      public void updateConstraintProps(SKBaseConstraint con, SKPropArray vPropList)
      {
         super.updateConstraintProps(con,vPropList);
      
         vPropList.add( frameMain.propFactory.getNewComboProp(Name,con.getConInfo(this,0),1,ConType) );
      }
   
      public int getConPropInfo(SKBaseConstraint con, String PropName, Object PropData)
      {
         if (PropData=="Center")
            return 1;
         else if(PropData=="Circumference")
            return 2;
         else 
            return 0;
      }
   
      public void repaint()
      {
         frameMain.RefreshShapeArea( ellipseShape.getBounds() );
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
   
      public int bifurUpdate(float[] data, int startIdx, int addX, int addY, float scale)
      {
         //startIdx = center.bifurUpdate(data,startIdx,addX,addY,scale);
         center.setShapePoint(new Point((int)(data[startIdx]*scale)+addX,(int)(data[startIdx+1]*scale)+addY));
         radius = data[startIdx+2];
         repaint();
         return startIdx+3;
      }
   
      public int bifurDataCount()
      {
         return center.bifurDataCount() + 1;
      }
      public void setShapePoint(Point pt)
      {
         int x1 = center.getX();
         int y1 = center.getY();
         int diffx = this.getShapeX()-pt.x;
         int diffy = this.getShapeY()-pt.y;
         center.setShapePoint(new Point(x1-diffx,y1-diffy));
      }
   
      public float distance(int x1, int y1, int x2, int y2)
      
      
      
      {
         return (float)java.lang.Math.sqrt(java.lang.Math.pow((double)(x1-x2),2)+java.lang.Math.pow((double)(y1-y2),2));
      }
      public void cloneShape(SKBaseShape sh)
      {
         super.cloneShape(sh);

         SKCircleShape cir = (SKCircleShape)sh;
         cir.radius = radius;
         center.cloneShape(cir.center);
      }

   
   }
