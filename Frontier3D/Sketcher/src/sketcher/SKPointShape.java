/**
 * Title:        SKPointShape
 * Description:  Just a point, represented as a circle.
 */
   package sketcher;
   import java.awt.event.*;
   import java.awt.Cursor;
   import java.awt.Point;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;
   import java.awt.Graphics;
   import java.awt.Color;

   public class SKPointShape extends SKNormalShape
   {
	public static SKMainFrame frameMain;
	public Color color = Color.blue;
	public int colorInd = 0;
	public static final String[] colArr  = new String[] {"Blue","Cyan","Green","Magenta","Orange","Pink"};
	public double radius = 0.2;
   
      public SKPointShape(SKMainFrame frame, int id, int x, int y, int z)
      {
         super(frame,id,"Point"+id,"Point",0,x,y);
         frameMain=frame;
         IconPath = "/images/pointicon.gif";
         reshape(x-10,y-10,20,20); //Subtract half of width and heigth so x,y is center
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
	    vPropList.add(frameMain.propFactory.getNewComboProp("Color",colorInd,0,colArr));
	    vPropList.add(frameMain.propFactory.getNewProp("Radius", Double.toString(radius),1));
         }
      
         if (vCurrentConstraints != null)
         { //Update constraints
            vCurrentConstraints.clear();
            ConstraintList.copyArrayTo(vCurrentConstraints);
         }
         super.updateMainShapeData(vPropList,vCurrentConstraints);         
      }
      public boolean setShapeData(String PropName, Object PropData)
      {
         if (super.setShapeData(PropName,PropData))
            return true;

         if (PropName=="Color")
         {
            String s = PropData.toString();
	    /*String t="";
	    int count=0,start=0;
	    float[] f = new float[3];
	    for(int i=0; i<s.length(); i++)
	    {
	      if(s.charAt(i) == ',')
	        {
        	  t = s.substring(start,i);
	          start = i+1;
		  if(!t.equals(""))
		    f[count++] = Float.parseFloat(t);
  		}
	     }
	    t = s.substring(start,s.length());
	    f[count++] = Float.parseFloat(t);
	    if(count == 3)
	    {  color = new Color(f[0],f[1],f[2]);
	       shape3d.changeColor(color);
	       System.out.println("111 color = "+f[0]+" "+f[1]+" "+f[2]);	
	    }*/
	    if(s.equals("Blue"))
		{color = Color.blue;
		colorInd = 0;}
	    if(s.equals("Cyan"))
                {color = Color.cyan;
		colorInd = 1;}
	    if(s.equals("Green"))
                {color = Color.green;
		colorInd = 2;}
            if(s.equals("Magenta"))
                {color = Color.magenta;
		colorInd = 3;}
            if(s.equals("Orange"))
                {color = Color.orange;
		colorInd = 4;}
            if(s.equals("Pink"))
                {color = Color.pink;
		colorInd = 5;}
	    shape3d.changeColor(color); 
	    repaint();
         }
         else if (PropName=="Radius")
         {
            radius = Double.parseDouble(PropData.toString());
	    ((SKPointShape3d)shape3d).changeRadius(radius);
	    ((SKPointShape3d)shape3d).radius=radius;
         }
         else
            return false;

         return true;
      }
   
      public void paintComponent(Graphics g)
      {
         if (shouldDraw())
         {
            updateColors();
         
         //Paint shape
            g.setColor(bodyColor);
            g.fillOval(5,5,10,10); //Make the shape smaller than the region (so point is small, but has mouse buffer zone)
	 
            if (bodyColor != outlineColor)
            {
               g.setColor(outlineColor);
               g.drawOval(5,5,10,10);
               //g.drawString("P"+ID,15,15);
            }
         }
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
               return this;
            default: 
               return this;
         }
      }
   
      public void paintSpecial(Graphics g)
      {
         if (getSelectable().highestGroup != null)
         {
            g.setColor(Color.black);
            g.drawString( new Integer( getSelectable().highestGroup.ID ).toString(), getX()+18, getY()+18 );
         }
      }
   
      public void updateConstraintProps(SKBaseConstraint con, SKPropArray vPropList)
      {
         super.updateConstraintProps(con,vPropList);
      
         vPropList.add( frameMain.propFactory.getNewProp(Name,Name,1,false) );
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
	 bodyColor = color;
      
      }
	
      public void writeAdditionalProps(DataOutputStream p) throws IOException
      {
         super.writeAdditionalProps(p);
	 p.writeDouble(radius);
	 p.writeInt(color.getRed());
	 p.writeInt(color.getGreen());
	 p.writeInt(color.getBlue());
      }
   
      public void readAdditionalProps(DataInputStream p) throws IOException
      {
         super.readAdditionalProps(p);
         radius = p.readDouble();
	 color = new Color(p.readInt(),p.readInt(),p.readInt());
      }	

   //Should this shape be drawn?
   
      public boolean shouldDraw()
      
      {  
         return (!frameMain.drawGroupRects || frameMain.selectedGroupShapes.indexOf(this) > -1);
      }
   
   
   }
