/**
 * Title:        SKLineShape
 * Description:  Represents a line with 2 points (segment, ray, line)
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Color;
   import java.awt.Point;
   import java.awt.geom.Line2D;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;

   public class SKLineShape extends SKBaseShape
   {
      public static final String[] PtType  = new String[] {"Point","Infinite"};
      public static final String[] ConType = new String[] {"Arbitrary","Pt1","Pt2"};
      public static String[] lineType = new String[] {"1.0","2.0","3.0"};
      public Color color = Color.blue;
      public double length = -1;
      public int type = 0;
      public SKPointLineShape pt1 = null,
      pt2 = null;
      public Line2D.Float     lineShape;
      public static SKMainFrame frameMain;
   
      public SKLineShape(SKMainFrame frame, int newID, int x, int y, int z)
      {
         super(frame,newID,"Line"+newID,"Line",3);
      //Create points
         pt1 = new SKPointLineShape(frame,newID+1,this,x-15,y);
         pt2 = new SKPointLineShape(frame,newID+2,this,x+15,y);
         lineShape = new Line2D.Float(x-15,y,x+15,y);
         frameMain=frame;
      
         IconPath = "/images/lineicon.gif";
      }
   
      public void severTies()
      {
         super.severTies();
         pt1 = null;
         pt2 = null;
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
            vPropList.add(frameMain.propFactory.getNewProp("Length", Double.toString(length),1,Color.red));
            vPropList.add(frameMain.propFactory.getNewComboProp("Pt1 Type",pt1.pointType,1,PtType) );
            vPropList.add(frameMain.propFactory.getNewProp("Pt1 X", Integer.toString(pt1.getShapeX()),1) );
            vPropList.add(frameMain.propFactory.getNewProp("Pt1 Y", Integer.toString(pt1.getShapeY()),1) );
            vPropList.add(frameMain.propFactory.getNewComboProp("Pt2 Type",pt2.pointType,1,PtType) );
            vPropList.add(frameMain.propFactory.getNewProp("Pt2 X", Integer.toString(pt2.getShapeX()),1) );
            vPropList.add(frameMain.propFactory.getNewProp("Pt2 Y", Integer.toString(pt2.getShapeY()),1) );
	    vPropList.add(frameMain.propFactory.getNewComboProp("Type",type,0,lineType));
         }
      }
   
      public boolean setShapeData(String PropName, Object PropData)
      {
         if (super.setShapeData(PropName,PropData)) 
            return true;
      
         if (PropName=="Length")
         {
            length = new Double(PropData.toString()).doubleValue();
         }
         else if (PropName=="Pt1 X")
         {
            pt1.setLocation(new Integer(PropData.toString()).intValue()-(pt1.getWidth()>>1),pt1.getY());
         }
         else if (PropName=="Pt1 Y")
         {
            pt1.setLocation(new Integer(PropData.toString()).intValue()-(pt1.getWidth()>>1),pt1.getY());
         }
         else if (PropName=="Pt2 X")
         {
            pt2.setLocation(new Integer(PropData.toString()).intValue()-(pt2.getWidth()>>1),pt2.getY());
         }
         else if (PropName=="Pt2 Y")
         {
            pt2.setLocation(new Integer(PropData.toString()).intValue()-(pt2.getWidth()>>1),pt2.getY());
         }
         else if (PropName=="Pt1 Type")
         {
            if (PropData.toString()=="Point")
               pt1.setPointType(0);
            else
               pt1.setPointType(1);
         }
         else if (PropName=="Pt2 Type")
         {
            if (PropData.toString()=="Point")
               pt2.setPointType(0);
            else
               pt2.setPointType(1);
         }
	 else if (PropName=="Type")
         {
            if(PropData.toString().equals("1.0"))
                {color = Color.blue;   
                type = 0;}
            if(PropData.toString().equals("2.0"))
                {color = Color.magenta;
                type = 1;}
            if(PropData.toString().equals("3.0"))
                {color = Color.orange;
                type = 2;}
            shape3d.changeColor(color);
            repaint();
         }
         else 
            return false;
      
      //Make panelShapeArea repaint
         frameMain.RefreshShapeArea();
      
         return true;
      }
   
      public void paintSpecial(Graphics g)
      {//System.out.println("entered line paintspecial");
         updateColors();
         g.setColor(outlineColor);
         switch(pt1.pointType+pt2.pointType)
         {
            case 0: g.drawLine(pt1.getX()+(pt1.getWidth()>>1),pt1.getY()+(pt1.getHeight()>>1),pt2.getX()+(pt2.getWidth()>>1),pt2.getY()+(pt2.getHeight()>>1) );
               break;
            case 1:         
               { 
                  SKPointShape thisPt = null,
                  otherPt = null;
                  if (pt1.pointType==1) 
                  { thisPt = pt1;
                     otherPt = this.getOtherPoint(pt1);}
                  else {thisPt = pt2;
                     otherPt = this.getOtherPoint(pt2);}
                  float x1 = thisPt.getShapeX(),
                  y1 = thisPt.getShapeY(),
                  x2 = otherPt.getShapeX(),
                  y2 = otherPt.getShapeY(),
                  slope = (y2-y1)/(x2-x1);
               
                  if(x1>x2)
                  {              
                     while(x1<2*(frameMain.getWidth()-x2))
                     {x1++;
                        y1 = y2 - (slope*(x2-x1));
                     }
                     g.drawLine((int)x1,(int)y1,(int)x2,(int)y2);
           	     lineShape.setLine(x1,y1,x2,y2);
                  }
                  else if(x1<x2)
                  {                     
                     while(x1>-(frameMain.getWidth()))
                     {x1--;
                        y1 = y2 - (slope*(x2-x1));}
                     g.drawLine((int)x1,(int)y1,(int)x2,(int)y2);
           	     lineShape.setLine(x1,y1,x2,y2);
                  }
                  else
                  {
                     if(y1>y2)
                     {                     
                        while(y1<2*(frameMain.getHeight()-y2))
                        {y1++;
                           x1 = x2 - ((y2-y1)/slope);}
                        g.drawLine((int)x1,(int)y1,(int)x2,(int)y2);
            	        lineShape.setLine(x1,y1,x2,y2);
                     }
                     else if(y1<y2)
                     {                     
                        while(y1>-(frameMain.getHeight()))
                        {y1--;
                           x1 = x2 - ((y2-y1)/slope);}
                        g.drawLine((int)x1,(int)y1,(int)x2,(int)y2);
           	     	lineShape.setLine(x1,y1,x2,y2);
                     }
                  }
               }
               break;
            case 2:
               { float X1=pt1.getShapeX(), X2=pt2.getShapeX(), Y1=pt1.getShapeY(), Y2=pt2.getShapeY(), slope = (Y2-Y1)/(X2-X1), x1=X1, x2=X2, y1=Y1, y2=Y2;
                  if(X1>X2)
                  {
                     while((x1 < 2*(frameMain.getWidth()-X2+10)) && x2>-(10+(frameMain.getWidth())))
                     {x1++;
                        x2--;
                        y1 = Y2 - (slope*(X2-x1));
                        y2 = Y1 + (slope*(x2-X1));
                     }
                     g.drawLine((int)x1,(int)y1,(int)x2,(int)y2);
           	     lineShape.setLine(x1,y1,x2,y2);
                  }
                  else if (X1<X2)
                  {
                     while((x2 < 2*(frameMain.getWidth()-X1+10)) && x1>-(10+(frameMain.getWidth())))
                     {x2++;
                        x1--;
                        y1 = Y2 - (slope*(X2-x1));
                        y2 = Y1 + (slope*(x2-X1));
                     }
                     g.drawLine((int)x1,(int)y1,(int)x2,(int)y2);
           	     lineShape.setLine(x1,y1,x2,y2);
                  }
                  else 
                  {
                     if(Y1<Y2)
                     {
                        while(y1>-(10+frameMain.getHeight()) && y2<(2*(frameMain.getHeight()-Y1+10)))
                        {y1--;
                           y2++;
                           x1 = X2 - ((Y2-y1)/slope);
                           x2 = X1 + ((y2-Y1)/slope);}
                        g.drawLine((int)x1,(int)y1,(int)x2,(int)y2);
           	        lineShape.setLine(x1,y1,x2,y2);
                     }
                     else if(Y1>Y2)
                     {                     
                        while(y2>-(10+frameMain.getHeight()) && y1<(2*(frameMain.getHeight()-Y2+10)))
                        {y2--;
                           y1++;
                           x1 = X2 - ((Y2-y1)/slope);
                           x2 = X1 + ((y2-Y1)/slope);}
                        g.drawLine((int)x1,(int)y1,(int)x2,(int)y2);
               	        lineShape.setLine(x1,y1,x2,y2);
                     }
                  }
               }
               break;
         }
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
            //outlineColor = SKOptions.colorOptions[SKOptions.shapeOutlineColor];
	    outlineColor = color;
         }
      }
   
   //Should this shape be drawn?
      public boolean shouldDraw()
      { 
      
         return (!frameMain.drawGroupRects || frameMain.selectedGroupShapes.indexOf(this) > -1);
      }
   
   
      public void setSelected(boolean sel)
      {
         if (sel != Selected)
         {
            Selected = sel;
            pt1.setSelected(sel);
            pt2.setSelected(sel);
         }
      }
   
      public SKPointShape getOtherPoint(SKPointShape pt)
      {
         return (pt.equals(pt1)) ? pt2 : pt1;
      }
   
      public SKPointShape getLeftPoint()
      {
         return (pt1.getX()<pt2.getX()) ? pt1 : pt2;
      }
   
      public SKPointShape getTopPoint()
      {
         return (pt1.getY()<pt2.getY()) ? pt1 : pt2;
      }
   
      public void doMove(int deltaX, int deltaY, boolean resolveConstraints)
      {
         pt1.doMove(deltaX,deltaY,false);
         pt2.doMove(deltaX,deltaY,resolveConstraints);
      }
   
      public boolean doHitTest(int x, int y)
      {
         return lineShape.intersects(x-3,y-3,6,6);
      }
   
      public int getNumConstraintPoints(SKBaseConstraint con)
      {
         return 2;
      }
   
      public int getNumSubShapes()
      {
         return 2;
      }
   
      public SKBaseShape getSubShape(int index)
      {
         switch (index)
         {
            case 1 : 
               return pt1;
            case 2 : 
               return pt2;
            default: 
               return this;
         }
      }
      public SKPointShape getConstraintSubShape(SKBaseConstraint con, int mod)
      {
         SKPointShape p;
         switch (mod)
         {
            case 1: p=pt1;
               break;
            case 2: p=pt2;
               break;
            default: p=pt1;
               break;
         }
         return p;
      }
      public Point getConstraintPoint(SKBaseConstraint con, int mod)
      {
         Point pt = new Point();
      
         switch (mod)
         {
            case  1:  
               { //Point 1
                  pt.x=pt1.getShapeX();
                  pt.y=pt1.getShapeY();
               }
               break;
            case  2:  
               { //Point 2
                  pt.x=pt2.getShapeX();
                  pt.y=pt2.getShapeY();
               }
               break;
            default:  
               { //Arbitrary
                  pt.x=(pt1.getShapeX()+pt2.getShapeX())/2;
                  pt.y=(pt1.getShapeY()+pt2.getShapeY())/2;
                  if(con instanceof SKDistanceConstraint) ((SKDistanceConstraint)con).perp = true;
               }
               break;
         }
      
         return pt;
      }
   
      public void setConstraintPoint(SKBaseConstraint con, int mod, Point pt)
      {
         switch (mod)
         {
            case 1: pt1.setShapePoint(pt);
               break;
            case 2: pt2.setShapePoint(pt);
         }
      }
   
      public int getConstraintMod(SKBaseShape sh)
      {
         if (sh == pt1)
            return 1;
         else if (sh == pt2)
            return 2;
         else
            return 0;
      }
   
      public void updateConstraintProps(SKBaseConstraint con, SKPropArray vPropList)
      {
         super.updateConstraintProps(con,vPropList);
      
         vPropList.add( frameMain.propFactory.getNewComboProp(Name,con.getConInfo(this,0),1,ConType) );
      }
   
      public int getConPropInfo(SKBaseConstraint con, String PropName, Object PropData)
      {
         if (PropData=="Pt1")
            return 1;
         else if (PropData=="Pt2")
            return 2;
         else
            return 0;
      }
   
      public void updateFromUTU(char vr, char mod, double deltaX, double deltaY, double value)
      {
         switch (mod)
         {
            case 'a' : pt1.updateFromUTU(vr,mod,deltaX,deltaY,value);
               break;
            case 'b' : pt2.updateFromUTU(vr,mod,deltaX,deltaY,value);
         }
      }
   
      public void repaint()
      {
         frameMain.RefreshShapeArea( (int)java.lang.Math.min(lineShape.x1,lineShape.x2)-1,(int)java.lang.Math.min(lineShape.y1,lineShape.y2)-1,(int)java.lang.Math.abs(lineShape.x1-lineShape.x2)+1,(int)java.lang.Math.abs(lineShape.y1-lineShape.y2)+1 );
      }
   
      public Point getPointForDistance(SKBaseShape otherShape)
      {
         return new Point(pt1.getShapeX(),pt2.getShapeY());
      }
      public int getY()
      {
         return (pt1.getY()+pt2.getY())/2;}
   
      public int getX()
      {
         return (pt1.getX()+pt2.getX())/2;}
   
      public void writeAdditionalProps(DataOutputStream p) throws IOException
      {
         super.writeAdditionalProps(p);
      
         p.writeDouble(length);
         pt1.writeAdditionalProps(p);
         pt2.writeAdditionalProps(p);
      }
   
      public void readAdditionalProps(DataInputStream p) throws IOException
      {
         super.readAdditionalProps(p);
         length = p.readDouble();
         pt1.readAdditionalProps(p);
         pt2.readAdditionalProps(p);
         SKLineShape3d newShape3d = new SKLineShape3d(frameMain,this.ID,1,1,1);
         frameMain.allshapes3d.add(newShape3d);
      }
      public void readAdditionalProps(DataInputStream p, int x, int y) throws IOException
      
      {
         super.readAdditionalProps(p);
      
         length = p.readDouble();
         pt1.readAdditionalProps(p,x,y);
         frameMain.repFirst=false;
         pt2.readAdditionalProps(p,x,y);
      }
   
      public boolean isPrimaryShape(SKBaseShape sh)
      {
         return (pt1==sh);
      }
   
      public int bifurUpdate(float[] data, int startIdx, int addX, int addY, float scale)
      {
         startIdx = pt1.bifurUpdate(data,startIdx,addX,addY,scale);
         //float m = data[startIdx];
         //pt2.setShapePoint(new Point((int)(pt1.getShapeX()+addX),(int)(m*(pt1.getShapeX()+addY))));
         startIdx = pt2.bifurUpdate(data,startIdx,addX,addY,scale);
         return startIdx+1;
      }
   
      public int bifurDataCount()
      {
         return pt1.bifurDataCount() + pt2.bifurDataCount() ;
      }
   
      public void cloneShape(SKBaseShape sh)
      {
         super.cloneShape(sh);
      
         SKLineShape line = (SKLineShape)sh;
         line.length = length;
         pt1.cloneShape(line.pt1);
         pt2.cloneShape(line.pt2);
      }
   
      public void setShapePoint(Point pt)
      {
         int x1 = pt1.getShapeX();
         int y1 = pt1.getShapeY();
         int x2 = pt2.getShapeX();
         int y2 = pt2.getShapeY();
         int diffx = this.getShapeX()-pt.x;
         int diffy = this.getShapeY()-pt.y;
         pt1.setShapePoint(new Point(x1-diffx,y1-diffy));
         pt2.setShapePoint(new Point(x2-diffx,y2-diffy));
      
      }
   }
