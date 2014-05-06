/**
 * Title:        SKTorsionConstraint
 * Description:  Torsion Constraint
 */
   package sketcher;
   import java.awt.Graphics;
   import java.awt.Color;
   import java.awt.geom.Line2D;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;

   public class SKTorsionConstraint extends SKBaseConstraint
   {
      public int count;
      public double angle = 60;
      public int axisInd1 = 0, axisInd2 = 1;
      public static String[] axisArr = new String[4];
      public SKBaseShape axis1 = null;
      public SKBaseShape axis2 = null;
      public SKMainFrame  frame= null;
      public SKTorsionConstraint(SKMainFrame frameMain, int idnum)
      {
         super(frameMain,idnum,"Torsion"+idnum,"Torsion",6);
         frame = frameMain;
         autoAddShapeProps = false;
         drawMode = 1;
      }
   
      public void updateMainProperties(SKPropArray vPropList)
      {
         super.updateMainProperties(vPropList);
	 vPropList.add(frameMain.propFactory.getNewProp("Angle", Double.toString(angle),0));	 
	 vPropList.add(frameMain.propFactory.getNewComboProp("Axis pt 1",axisInd1,0,axisArr));
	 vPropList.add(frameMain.propFactory.getNewComboProp("Axis pt 2",axisInd2,0,axisArr));
      }

      public boolean setMainData(String PropName, Object PropData)
      {
         if (super.setMainData(PropName,PropData))
            return true;

         if (PropName=="Angle")
         {
            angle = new Double(PropData.toString()).doubleValue();
         }
	 else if(PropName=="Axis pt 1")
	 {
	    String s = PropData.toString();	    
	    if(s.equals(ShapeList.get(0).Name))
		{axis1 = ShapeList.get(0);
		axisInd1 = 0;}
	    else if(s.equals(ShapeList.get(1).Name))
                {axis1 = ShapeList.get(1);
		axisInd1 = 1;}
	    else if(s.equals(ShapeList.get(2).Name))
                {axis1 = ShapeList.get(2);
		axisInd1 = 2;}
	    else if(s.equals(ShapeList.get(3).Name))
		{axis1 = ShapeList.get(3);
		axisInd1 = 3;}
	 }
	 else if(PropName=="Axis pt 2")
	 {
	    String s = PropData.toString();	    
	    if(s.equals(ShapeList.get(0).Name))
		{axis2 = ShapeList.get(0);
		axisInd2 =0;}
	    else if(s.equals(ShapeList.get(1).Name))
                {axis2 = ShapeList.get(1);
		axisInd2 = 1;}
	    else if(s.equals(ShapeList.get(2).Name))
                {axis2 = ShapeList.get(2);
		axisInd2 = 2;}
	    else if(s.equals(ShapeList.get(3).Name))
		{axis2 = ShapeList.get(3);
		axisInd2 = 3;}
	 }
         else
            return false;
	 repaint();
         return true;
      }
   
      public static boolean isAvailable(SKShapeArray selectedShapes)
      {
         return ( (selectedShapes.get(0) instanceof SKPointShape) && (selectedShapes.get(1) instanceof SKPointShape) && (selectedShapes.get(2) instanceof SKPointShape) && (selectedShapes.get(3) instanceof SKPointShape));
      }
   
   
      public void doPaint(Graphics g)
      
      {
         super.doPaint(g);
      }
   
   
      public void drawConstraint(Graphics g,int x1,int y1,int x2,int y2)
      { 
         super.drawConstraint(g,x1,y1,x2,y2);
         int centerX=(int)(axis1.getShapeX()+(axis2.getShapeX()-axis1.getShapeX())/2), 
	     centerY=(int)(axis1.getShapeY()+(axis2.getShapeY()-axis1.getShapeY())/2);
	 g.drawArc(centerX-15,centerY-15,30,30,240,-300);
	 g.drawLine(axis1.getShapeX(),axis1.getShapeY(),axis2.getShapeX(),axis2.getShapeY());
      }
	
      public boolean doHitTest(int x, int y)
      {
	 Line2D.Double line = new Line2D.Double();
         line.setLine(axis1.getShapeX(),axis1.getShapeY(),axis2.getShapeX(),axis2.getShapeY());

         return line.intersects(x-3,y-3,6,6);
      }
     
      public void readAdditionalProps(DataInputStream p) throws IOException
      {
         super.readAdditionalProps(p);
         angle = p.readDouble();
	 axisInd1 = p.readInt();
	 axisInd2 = p.readInt();
      }

      public void writeAdditionalProps(DataOutputStream p) throws IOException
      {
         super.writeAdditionalProps(p);
         p.writeDouble(angle);
	 p.writeInt(axisInd1);
	 p.writeInt(axisInd2);
      }
   }
