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

   import javax.media.j3d.*;
   import javax.vecmath.*;
   import com.sun.j3d.utils.geometry.*;

   public class SKAngleConstraint3d extends SKNormalConstraint3d
   {
      public LineArray    lineShape;
      public double angle = 60;
      public Point3d[] vertices = new Point3d[4];
      public Color3f[] colors = new Color3f[4];
      public TransformGroup group = new TransformGroup();
   //      int config3 = 1;
   
      public SKAngleConstraint3d(SKMainFrame MainFrame, int idnum)
      {
         super(MainFrame,idnum,"Angle"+idnum,"Angle",7);
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
   
      public void doPaint()
      {
         super.doPaint();
         if (!isDrawn)
            return;
         SKBaseShape3d sh1 = ShapeList.get(0);
         SKBaseShape3d sh2 = ShapeList.get(1);
         if(ShapeList.size()>2)
         {
            SKBaseShape3d sh3 = ShapeList.get(2);
         
            Point3d pt1 = sh1.getConstraintPoint(this,config1),
            pt2 = sh2.getConstraintPoint(this,config2),
            pt3 = sh3.getConstraintPoint(this,config3);
         //System.out.println("111 points = "+sh1+" "+sh2+" "+sh3);
         //System.out.println("111 angle3d "+this+" "+pt1+" "+pt2+" "+pt3);
         
            drawConstraint(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y, pt2.z,pt3.x,pt3.y,pt3.z, frameMain.panelShapeArea3d);
         }
      }
      public void doPaint(JSelectionPanel3d panel)
      {
         super.doPaint();
         if (!isDrawn)
            return;
         SKBaseShape3d sh1 = ShapeList.get(0);
         SKBaseShape3d sh2 = ShapeList.get(1);
         if(ShapeList.size()>2)
         {
            SKBaseShape3d sh3 = ShapeList.get(2);
         
            Point3d pt1 = sh1.getConstraintPoint(this,config1),
            pt2 = sh2.getConstraintPoint(this,config2),
            pt3 = sh3.getConstraintPoint(this,config3);
         //System.out.println("111 points = "+sh1+" "+sh2+" "+sh3);
         //System.out.println("111 angle3d "+this+" "+pt1+" "+pt2+" "+pt3);
         
            drawConstraint(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y, pt2.z,pt3.x,pt3.y,pt3.z,panel);
         }
      }
   
   
      public void drawConstraint(double x1,double y1,double z1,double x2,double y2, double z2, double x3,double y3, double z3, JSelectionPanel3d panel)
      {
         if (frameMain.panelShapeArea.DrawConstraints && frameMain.drawConstrDetails && create)
         {
            vertices[0] = new Point3d(x1, y1, z1);
            vertices[1] = new Point3d(x2, y2, z2);
            vertices[2] = new Point3d(x2, y2, z2);
            vertices[3] = new Point3d(x3, y3, z3);
         
            colors[0]=new Color3f(0.0f,0.0f,1.0f);
            colors[1]=new Color3f(0.0f,0.0f,1.0f);
            colors[2]=new Color3f(0.0f,0.0f,1.0f);
            colors[3]=new Color3f(0.0f,0.0f,1.0f);
         
            lineShape = new LineArray(4,GeometryArray.COORDINATES|GeometryArray.COLOR_3);
            lineShape.setCoordinates(0,vertices);
            lineShape.setColors(0,colors);
            lineShape.setCapability(LineArray.COLOR_3);
            lineShape.setCapability(LineArray.ALLOW_COLOR_WRITE);
            lineShape.setCapability(LineArray.ALLOW_COORDINATE_WRITE);
            lineShape.setCapability(Geometry.ALLOW_INTERSECT);
            Shape3D shape = new Shape3D(lineShape);
            Appearance app1 = new Appearance();
            LineAttributes att = new LineAttributes(4.5f,LineAttributes.PATTERN_DASH,true );
            app1.setLineAttributes(att);
            shape.setAppearance(app1);
            group.addChild(shape);
            panel.addObject(group);
            create=false;
         }
         else if (frameMain.panelShapeArea.DrawConstraints && frameMain.drawConstrDetails)
         {
	    Vector3f v1 = new Vector3f((float)(x1-x2), (float)(y1-y2), (float)(z1-z2));
	    Vector3f v2 = new Vector3f((float)(x3-x2), (float)(y3-y2), (float)(z3-z2));
	    float mag1 = magnitude(v1);
	    float mag2 = magnitude(v2);
	    normalize(v1);
	    normalize(v2);
	    x1 = (float)(v1.x+x2);	
	    y1 = (float)(v1.y+y2);	
	    z1 = (float)(v1.z+z2);	
	    x3 = (float)(v2.x+x2);	
	    y3 = (float)(v2.y+y2);	
	    z3 = (float)(v2.z+z2);	

            vertices[0] = new Point3d(x1, y1, z1);
            vertices[1] = new Point3d(x2, y2, z2);
            vertices[2] = new Point3d(x2, y2, z2);
            vertices[3] = new Point3d(x3, y3, z3);
            lineShape.setCoordinates(0,vertices);
         }
      }
   
      public static boolean isAvailable(SKShapeArray selectedShapes)
      {
         return true;
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
   }
