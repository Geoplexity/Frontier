/**
 * Title:        SKDistanceConstraint
 * Description:  Distance Constraint
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Color;
   import java.awt.geom.Line2D;
   import java.awt.Point;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;
   import javax.swing.JLabel;

   public class SKDistanceConstraint extends SKNormalConstraint
   {
      public static String[] distType = new String[] {"1.0","2.0","3.0"};
      public static Line2D.Double line = new Line2D.Double();
      public int type = 0;
      public Color color = Color.black;
      public double distance = 1;
      public boolean perp = false;
   
      public SKDistanceConstraint(SKMainFrame MainFrame, int idnum)
      {
         super(MainFrame,idnum,"Distance"+idnum,"Distance",0);
      }
   
      public void updateMainProperties(SKPropArray vPropList)
      {
         super.updateMainProperties(vPropList);
      
         vPropList.add(frameMain.propFactory.getNewProp("Distance", Double.toString(distance),0));
	 vPropList.add(frameMain.propFactory.getNewComboProp("Type",type,0,distType));
      }
   
      public boolean setMainData(String PropName, Object PropData)
      {
         if (super.setMainData(PropName,PropData))  
            return true;
      
         if (PropName=="Distance")
         {
            distance = new Double(PropData.toString()).doubleValue();
	    repaint();
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
	    constraint3d.changeColor(color);
	    repaint();
	 }
         else 
            return false;
      
         return true;
      }
   
      public void doPaint(Graphics g)
      {
         if (!isDrawn)  
            return;
         SKBaseShape sh1 = ShapeList.get(0);
         SKBaseShape sh2 = ShapeList.get(1);
      
         Point pt1 = sh1.getConstraintPoint(this,config1),
         pt2 = sh2.getConstraintPoint(this,config2);
      	 
	 //System.out.println("111 cons "+this);
         drawConstraint(g,pt1.x,pt1.y,pt2.x,pt2.y);
      }
   
      public void drawConstraint(Graphics g,int x1,int y1,int x2,int y2)
      {
         super.drawConstraint(g,x1,y1,x2,y2);
	if(!((!frameMain.editingShape && this==frameMain.SelectedConstraint) || (this==frameMain.mouseOverItem)))
	      g.setColor(color);
         boolean arbit1=false, arbit2=false;
	 if( perp)
         {
            double slope1=0,c1=0,slope2=0,c2=0;
            boolean isFound = true;
            if((ShapeList.get(0) instanceof SKLineShape)||(ShapeList.get(0) instanceof SKImageShape))
            {isFound = false;
               for(int j=0; j<ShapeList.get(0).getNumSubShapes(); j++)
               {
                  SKBaseShape s=ShapeList.get(0).getSubShape(j);
                  isFound = isFound || ((s.getShapeX()==x1)&&(s.getShapeY()==y1));
               }
            }
            if(!isFound)arbit1=true;
            isFound=true;
            if((ShapeList.get(1) instanceof SKLineShape)||(ShapeList.get(1) instanceof SKImageShape))
            {
               isFound = false;
               for(int j=0; j<ShapeList.get(1).getNumSubShapes(); j++)
               {
                  SKBaseShape s=ShapeList.get(1).getSubShape(j);
                  isFound = isFound || ((s.getShapeX()==x2)&&(s.getShapeY()==y2));
               }
            }
            if(!isFound)arbit2=true;
         
            if(arbit1 )
            {
               SKLineShape sh1=null;
               SKImageShape Sh1=null;
               if(ShapeList.get(0) instanceof SKLineShape)
                  sh1=(SKLineShape)ShapeList.get(0);
               else if (ShapeList.get(0) instanceof SKImageShape)
                  Sh1=(SKImageShape)ShapeList.get(0);
            
            
               if(ShapeList.get(0) instanceof SKLineShape)
               {
                  slope1= (double)(sh1.pt2.getShapeY() - sh1.pt1.getShapeY()) / (sh1.pt2.getShapeX() - sh1.pt1.getShapeX());
                  c1 = sh1.pt2.getShapeY()  -(slope1*sh1.pt2.getShapeX());
                  slope2 = -1/slope1;
                  c2 = (y2  -(slope2*x2));
               }
               else if (ShapeList.get(0) instanceof SKImageShape)
               {	slope1= (double)(Sh1.pt2.getShapeY() - Sh1.pt1.getShapeY()) / (Sh1.pt2.getShapeX() - Sh1.pt1.getShapeX());
                  c1 = Sh1.pt2.getShapeY()  -(slope1*Sh1.pt2.getShapeX());
                  slope2 = -1/slope1;
                  c2 = (y2  -(slope2*x2));
               }
            
               double[] array = new double[4];
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
               SKLineShape sh2=null;
               SKImageShape Sh2=null;
               if (ShapeList.get(1) instanceof SKLineShape)
                  sh2=(SKLineShape)ShapeList.get(1);	
               else if  (ShapeList.get(1) instanceof SKImageShape)
                  Sh2=(SKImageShape)ShapeList.get(1);
               if(ShapeList.get(1) instanceof SKLineShape)
               {
                  slope1 = (double)(sh2.pt2.getShapeY() - sh2.pt1.getShapeY()) / (sh2.pt2.getShapeX() - sh2.pt1.getShapeX());
                  c1 = sh2.pt2.getShapeY()  -(slope1*sh2.pt2.getShapeX());
                  slope2=-1/slope1;
                  c2 = (y1  -(slope2*x1));
               }
               else if(ShapeList.get(1) instanceof SKImageShape)
               {
                  slope1 = (double)(Sh2.pt2.getShapeY() - Sh2.pt1.getShapeY()) / (Sh2.pt2.getShapeX() - Sh2.pt1.getShapeX());
                  c1 = Sh2.pt2.getShapeY()  -(slope1*Sh2.pt2.getShapeX());
                  slope2=-1/slope1;
                  c2 = (y1  -(slope2*x1));
               }
               double[] array = new double[4];
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
	 //System.out.println("111 drawn "+this+" "+x1+" "+y1+" "+x2+" "+y2);
         g.drawLine(x1,y1,x2,y2);
         //Draw value
         if (frameMain.panelShapeArea.DrawConstraints && frameMain.drawConstrDetails)
            g.drawString( Double.toString(distance), (java.lang.Math.abs(x1-x2)/2 + java.lang.Math.min(x1,x2))-10,(java.lang.Math.abs(y1-y2)/2 + java.lang.Math.min(y1,y2)) );
      }
   
      public void repaint()
      {
         frameMain.RefreshShapeArea( (int)java.lang.Math.min(line.x1,line.x2)-1,(int)java.lang.Math.min(line.y1,line.y2)-1,
                                 (int)java.lang.Math.abs(line.x1-line.x2)+1,(int)java.lang.Math.abs(line.y1-line.y2)+1 );
      }
   
      public boolean doHitTest(int x, int y)
      {
         Point pt1 = ShapeList.get(0).getConstraintPoint(this,config1),
         pt2 = ShapeList.get(1).getConstraintPoint(this,config2);
      
         line.setLine(pt1.x,pt1.y,pt2.x,pt2.y);
      
         return line.intersects(x-3,y-3,6,6);
      }
   
      public void writeAdditionalProps(DataOutputStream p) throws IOException
      {
         super.writeAdditionalProps(p);
      
         p.writeDouble( distance );
      }
   
      public void readAdditionalProps(DataInputStream p) throws IOException
      {
         super.readAdditionalProps(p);
      
         distance = p.readDouble();
      }
   }
