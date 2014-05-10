/**
 * Title:        SKAngleConstraint
 * Description:  Angle Constraint
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Color;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;
   import java.awt.Point;

   public class SKAngleConstraint extends SKNormalConstraint
   {
      public double angle = 60;
   
      public SKAngleConstraint(SKMainFrame MainFrame, int idnum)
      {
         super(MainFrame,idnum,"Angle"+idnum,"Angle",4);
      }
   
      public void updateMainProperties(SKPropArray vPropList)
      {
         super.updateMainProperties(vPropList);
      
         vPropList.add(frameMain.propFactory.getNewProp("Angle", Double.toString(angle),0));
      }
   
      public boolean setMainData(String PropName, Object PropData)
      {
         if (super.setMainData(PropName,PropData)) 
            return true;
      
         if (PropName=="Angle")
         {
            angle = new Double(PropData.toString()).doubleValue();
         }
         else
            return false;
      
         return true;
      }
   
      public void doPaint(Graphics g)
      {
         super.doPaint(g);
      }
   
      public void drawConstraint(Graphics g,int x1,int y1,int x2,int y2)
      {
         // if((this instanceof SKAngleConstraint) && (SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] == 0))
         //{
         super.drawConstraint(g,x1,y1,x2,y2);
	 if(SKOptions.byteOptions[SKOptions.simpleSolverMode]>0)
         if((ShapeList.get(0) instanceof SKLineShape) && (ShapeList.get(1) instanceof SKLineShape)) 
         {  
            SKLineShape line1 = (SKLineShape) ShapeList.get(0);
            SKLineShape line2 = (SKLineShape) ShapeList.get(1);
            double[] array=new double[2];
            double min;
            int num=0;
            SKPointShape pt=line2.pt1,p=line1.pt1;
            int x,y;
            double  slope1=1/0.0, slope2=1/0.0;
            if(line1.pt2.getShapeX()!= line1.pt1.getShapeX())
            {   slope1=((double)(line1.pt2.getShapeY()-line1.pt1.getShapeY()))/((double)(line1.pt2.getShapeX()-line1.pt1.getShapeX()));}
         
            if(line2.pt2.getShapeX()!= line2.pt1.getShapeX())
            {   slope2=((double)(line2.pt2.getShapeY()-line2.pt1.getShapeY()))/((double)(line2.pt2.getShapeX()-line2.pt1.getShapeX()));}
            double c1,c2 ;
            c1= p.getShapeY() -(slope1*p.getShapeX());
            c2= pt.getShapeY() -(slope2*pt.getShapeX());
            lineintersection(array,slope1,slope2,c1,c2);
            x=(int)array[0];
            y=(int)array[1];
            p = line1.getConstraintSubShape(this,this.getConInfo(line1,0)); 
            pt = line2.getConstraintSubShape(this,this.getConInfo(line2,0));
            g.drawLine(pt.getShapeX(),pt.getShapeY(),(int)x,(int)y);
            g.drawLine(p.getShapeX(),p.getShapeY(),(int)x,(int)y);

//g.drawArc(x-10,y-10,20,20,-(int)(java.lang.Math.atan(slope1)*180/3.14),-(int)((java.lang.Math.atan(slope2)-java.lang.Math.atan(slope1))*180/3.14));
         
            if (frameMain.panelShapeArea.DrawConstraints && frameMain.drawConstrDetails)
               g.drawString( Double.toString(angle), x+10, y+10);
         
         }
         else
         {
            if(!((ShapeList.get(0) instanceof SKImageShape)||(ShapeList.get(1) instanceof SKImageShape)))
            {
               Point p1, p2, p3;
               double  slope1=1/0.0, slope2=1/0.0;
               if(ShapeList.get(0) instanceof SKPointShape)
               {	
                  p1 = ShapeList.get(0).getConstraintPoint(this,0);
                  p2 = ShapeList.get(1).getConstraintPoint(this,((SKAngleConstraint)this).config2);
                  SKBaseShape s = ((SKLineShape)ShapeList.get(1)).getConstraintSubShape(this, ((SKAngleConstraint)this).config2);
                  SKPointShape p = ((SKLineShape)ShapeList.get(1)).getOtherPoint((SKPointShape)s);
                  p3 = new Point(p.getShapeX(), p.getShapeY());
               }
               else
               {
                  p1 = ShapeList.get(1).getConstraintPoint(this,0);
                  p2 = ShapeList.get(0).getConstraintPoint(this,((SKAngleConstraint)this).config1);
                  SKBaseShape s = ((SKLineShape)ShapeList.get(1)).getConstraintSubShape(this, ((SKAngleConstraint)this).config1);
                  SKPointShape p = ((SKLineShape)ShapeList.get(0)).getOtherPoint((SKPointShape)s);
                  p3 = new Point(p.getShapeX(), p.getShapeY());
               }
               slope1=(double)(p1.y-p2.y)/(double)(p1.x-p2.x);
               slope2=(double)(p3.y-p2.y)/(double)(p3.x-p2.x);
               g.drawLine(p1.x,p1.y,p2.x,p2.y);
               g.drawLine(p2.x,p2.y,p3.x,p3.y);

//g.drawArc(p2.x-10,p2.y-10,20,20,-(int)(java.lang.Math.atan(slope1)*180/3.14),-(int)((java.lang.Math.atan(slope2)-java.lang.Math.atan(slope1))*180/3.14));
            
               if (frameMain.panelShapeArea.DrawConstraints && frameMain.drawConstrDetails)
                  g.drawString( Double.toString(angle), p2.x+10, p2.y+10);
            
            }
         }
      
      }
      public void drawConstraint(Graphics g,int x1,int y1,int x2,int y2, int x3, int y3)
      {
         super.drawConstraint(g,x1,y1,x2,y2,x3,y3);
         g.drawLine(x1,y1,x2,y2);
         g.drawLine(x3,y3,x2,y2); 
         double  slope1=1/0.0, slope2=1/0.0;
         slope1=(double)(y1-y2)/(double)(x1-x2);
         slope2=(double)(y3-y2)/(double)(x3-x2);
	 //System.out.println("111 slope1 = "+slope1+" slope2 = "+slope2);
	 /*if(slope1>slope2)
           g.drawArc(x2-10,y2-10,20,20,(int)(java.lang.Math.atan(slope1)*180/3.14),-(int)((java.lang.Math.atan(slope2)+java.lang.Math.atan(slope1))*180/3.14));
	 else*/

//g.drawArc(x2-10,y2-10,20,20,-(int)(java.lang.Math.atan(slope1)*180/3.14),-(int)((java.lang.Math.atan(slope2)-java.lang.Math.atan(slope1))*180/3.14));
         if (frameMain.panelShapeArea.DrawConstraints && frameMain.drawConstrDetails)
            g.drawString( Double.toString(angle), x2+10, y2+10);
      }
   
      public static boolean isAvailable(SKShapeArray selectedShapes)
      {  boolean available;
         if(selectedShapes.size() == 3)
         {
            available = true;
            for (int i=0;i<3;i++)
               available = available && (selectedShapes.get(i) instanceof SKPointShape);
            return available;
         }
         available = ((selectedShapes.get(0) instanceof SKLineShape) && (selectedShapes.get(1) instanceof SKLineShape));
         available = available || ((selectedShapes.get(0) instanceof SKPointShape) && (selectedShapes.get(1) instanceof SKLineShape));
         available = available || ((selectedShapes.get(1) instanceof SKPointShape) && (selectedShapes.get(0) instanceof SKLineShape));
         available = available || ((selectedShapes.get(0) instanceof SKLineShape) && (selectedShapes.get(1) instanceof SKImageShape));
         available = available || ((selectedShapes.get(1) instanceof SKLineShape) && (selectedShapes.get(0) instanceof SKImageShape));
         available = available || ((selectedShapes.get(0) instanceof SKImageShape) && (selectedShapes.get(1) instanceof SKImageShape));
         return available;
      }
   
      public void writeAdditionalProps(DataOutputStream p) throws IOException
      {
         super.writeAdditionalProps(p);
      
         p.writeDouble( angle );
      }
   
      public void readAdditionalProps(DataInputStream p) throws IOException
      {
         super.readAdditionalProps(p);
      
         angle = p.readDouble();
      }
      public void cloneConstraint(SKBaseConstraint con, SKShapeArray shapeArray)
      {
         super.cloneConstraint(con,shapeArray);
         ((SKAngleConstraint)con).angle = angle;
      }
   }
