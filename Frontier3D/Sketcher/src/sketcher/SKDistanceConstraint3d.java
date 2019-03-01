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

   import com.sun.j3d.utils.universe.*;
   import com.sun.j3d.utils.geometry.*;
   import com.sun.j3d.utils.behaviors.mouse.*;
   import javax.media.j3d.*;
   import javax.vecmath.*;
   import javax.media.j3d.J3DGraphics2D;
   import com.sun.j3d.utils.image.TextureLoader;
   import com.sun.j3d.utils.behaviors.picking.*;


   public class SKDistanceConstraint3d extends SKNormalConstraint3d
   {
      public LineArray    lineShape;
      public Cylinder line;
      public double distance = 1;
      public boolean perp = false;
      public static String[] distType = new String[] {"1.0","2.0","3.0"};
      public Point3d[] vertices = new Point3d[2];
      public Color3f[] colors = new Color3f[2];
      public Color3f color = new Color3f(1.0f, 0.0f, 0.0f);
      public TransformGroup cTrans, cScale, point1_group, point2_group, line_group;
      public TransformGroup group = new TransformGroup();
      public Transform3D cTranslate, cRotate, scaleTrans;

   
      public SKDistanceConstraint3d(SKMainFrame MainFrame, int idnum)
      {
         super(MainFrame,idnum,"Distance"+idnum,"Distance",6);
	 Appearance app = new Appearance();
         Color3f ambientColour = new Color3f(0.2f, 0.2f, 0.2f);
         Color3f diffuseColour = new Color3f(1.0f, 0.0f, 0.0f);
         Color3f specularColour = new Color3f(1.0f, 1.0f, 1.0f);
         Color3f emissiveColour = new Color3f(0.0f, 0.0f, 0.0f);
         Material mat = new Material(ambientColour, emissiveColour, diffuseColour,
                           specularColour, 0.5f);
         mat.setShininess(70f);
         mat.setLightingEnable(true);
         app.setMaterial(mat);
	 line = new Cylinder(0.03f, 1f,  Cylinder.GENERATE_NORMALS | Primitive.GENERATE_TEXTURE_COORDS |
                            Primitive.ENABLE_GEOMETRY_PICKING|
                            Primitive.ENABLE_APPEARANCE_MODIFY, app);
         line.setCapability(Primitive.ENABLE_APPEARANCE_MODIFY);
         line.setCapability(Node.ENABLE_PICK_REPORTING);
	 line.setUserData(this.Name);
	 //System.out.println("111 created "+this);	
      }
   
      public TransformGroup addDistance(double x1,double y1,double z1, double x2,double y2, double z2)
      {
	scaleTrans =  new Transform3D();
        scaleTrans.setScale(new Vector3d(1.0,1.0,1.0));
        cScale = new TransformGroup();
        cScale.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
        cScale.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
        cScale.setTransform(scaleTrans);
        cScale.addChild(line);
        cTranslate =  new Transform3D();
        cRotate = new Transform3D();
        //cTranslate.setTranslation(new Vector3f((float)(x1+(x2-x1)/2),(float)(y1+(y2-y1)/2),(float)(z1+(z2-z1)/2)));
        SKBaseShape3d.adjustTransform(cTranslate);
	cRotate.setRotation(new AxisAngle4f(new Vector3f(0.0f, 0.0f, 1.0f), 1.57f));
        cTranslate.mul(cRotate);
        cTrans = new TransformGroup();
        cTrans.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
        cTrans.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
        cTrans.setTransform(cTranslate);
        cTrans.addChild(cScale);

        group.addChild(cTrans);
        group.setUserData(Name);

        drawConstraint(x1,y1,z1,x2,y2,z2);
	return group;
      }      

      public void updateMainProperties(SKPropArray vPropList)
      {
         super.updateMainProperties(vPropList);
      
         vPropList.add(frameMain.propFactory.getNewProp("Distance", Double.toString(distance),0));
      }
   
      public boolean setMainData(String PropName, Object PropData)
      {
         if (super.setMainData(PropName,PropData))
            return true;
      
         if (PropName=="Distance")
         {
            distance = new Double(PropData.toString()).doubleValue();
         }
         else
            return false;
      
         return true;
      }
   
      public TransformGroup getAddTrans()
      {
	 SKBaseShape3d sh1 = ShapeList.get(0);
         SKBaseShape3d sh2 = ShapeList.get(1);
	 Point3d pt1 = sh1.getConstraintPoint(this,config1),
         pt2 = sh2.getConstraintPoint(this,config2);
	 return addDistance(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y, pt2.z);
      }

      public void doPaint()
      {

         if (!isDrawn)
          return;
         SKBaseShape3d sh1 = ShapeList.get(0);
         SKBaseShape3d sh2 = ShapeList.get(1);
      
         Point3d pt1 = sh1.getConstraintPoint(this,config1),
         pt2 = sh2.getConstraintPoint(this,config2);
         drawConstraint(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y, pt2.z);
      }
      public void doPaint(JSelectionPanel3d panel)
      {
       
         if (!isDrawn)
          return;
         SKBaseShape3d sh1 = ShapeList.get(0);
         SKBaseShape3d sh2 = ShapeList.get(1); 

         Point3d pt1 = sh1.getConstraintPoint(this,config1),
         pt2 = sh2.getConstraintPoint(this,config2);
         drawConstraint(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y, pt2.z);
      }

   
      public void drawConstraint(double x1,double y1,double z1, double x2,double y2, double z2)
      {
	 //System.out.println("111 "+this);
	 //System.out.println("111 "+this+" "+x1+" "+y1+" "+z1+" "+x2+" "+y2+" "+z2);
	 Vector3f v1 = new Vector3f();
         Vector3f v2 = new Vector3f();  
         Vector3f v3 = new Vector3f();
         v1 = new Vector3f((float)x1, (float)y1, (float) z1);
         v2 = new Vector3f((float)x2, (float)y2, (float) z2);
         v3 = new Vector3f((float)x2, (float)y2, (float) z2);
         
         double newDistance = SKLineShape3d.distance(new Point3d(x1,y1,z1), new Point3d(x2,y2,z2));

         v2.x-=v1.x;
         v2.y-=v1.y;
         v2.z-=v1.z;
         SKLineShape3d.normalize(v2);
         float dProduct = SKLineShape3d.dotProduct(new Vector3f(0f,1.0f,0f), v2);
         double angle = java.lang.Math.acos(dProduct);
         Vector3f cProduct = SKLineShape3d.crossProduct(new Vector3f(0f,1.0f,0f), v2);
	 if(v2.x==0f && v2.y==1f && v2.z==0f) cProduct = v2;
         SKLineShape3d.normalize(cProduct);
         cRotate = new Transform3D();
         cRotate.setRotation(new AxisAngle4f(cProduct,(float)angle));
         double newScale = newDistance;
         float moveScale = (float) ((newDistance)/2);
         scaleTrans =  new Transform3D();
         scaleTrans.setScale(new Vector3d(1.0,newScale,1.0));
         Vector3f translate =  new Vector3f(v1.x, v1.y, v1.z);
         cTranslate = new Transform3D();  
         cTranslate.setTranslation(translate);
         
         Vector3f translate1 =  new Vector3f(0f, moveScale, 0);
         Transform3D cTranslate1 = new Transform3D();
         cTranslate1.setTranslation(translate1);

         cScale.setTransform(scaleTrans);
         cRotate.mul(cTranslate1);
         cTranslate.mul(cRotate);
         cTrans.setTransform(cTranslate);	    
	 distance = newDistance;
      }
      public void adjustVertices(Point3d[] vertices)
      {
         Transform3D t = frameMain.panelShapeArea3d.getView();
         Vector3f v = new Vector3f();
         t.get(v);
         if(frameMain.topView)
            v.y-=20.0f;
         else
            v.z-=20.0f;
         vertices[0] = new Point3d(vertices[0].x+v.x, vertices[0].y+v.y, vertices[0].z+v.z);
         vertices[1] = new Point3d(vertices[1].x+v.x, vertices[1].y+v.y, vertices[1].z+v.z);
      }
      public void repaint()
      {
      }

      public void changeColor(Color c)
      {
        float[] rgb = new float[3];
        c.getColorComponents(rgb);
        color = new Color3f(rgb[0],rgb[1],rgb[2]);
        Appearance app = new Appearance();
         Color3f ambientColour = new Color3f(0.2f, 0.2f, 0.2f);
         Color3f diffuseColour = new Color3f(rgb[0],rgb[1],rgb[2]);
         Color3f specularColour = new Color3f(1.0f, 1.0f, 1.0f);
         Color3f emissiveColour = new Color3f(0.0f, 0.0f, 0.0f);
         Material mat = new Material(ambientColour, emissiveColour, diffuseColour,
                              specularColour, 0.5f);
         mat.setShininess(70f);
         mat.setLightingEnable(true);
         app.setMaterial(mat);
         line.setAppearance(app);
      }
      public void changeColor(Color3f c)
      {
        color = c;
        Appearance app = new Appearance();
         Color3f ambientColour = new Color3f(0.2f, 0.2f, 0.2f);
         Color3f diffuseColour = color;
         Color3f specularColour = new Color3f(1.0f, 1.0f, 1.0f);
         Color3f emissiveColour = new Color3f(0.0f, 0.0f, 0.0f);
         Material mat = new Material(ambientColour, emissiveColour, diffuseColour,
                              specularColour, 0.5f);
         mat.setShininess(70f);
         mat.setLightingEnable(true);
         app.setMaterial(mat);
         line.setAppearance(app);
      }

      public void cloneConstraint(SKBaseConstraint3d con, SKShape3dArray shapeArray)
      {
	 super.cloneConstraint(con,shapeArray);
	 ((SKDistanceConstraint3d)con).color = color;
      }
   
      public boolean doHitTest(int x, int y)
      {
         return false;
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
