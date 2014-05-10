 /**
 * Title:        SKPointShape
 * Description:  Just a point, represented as a circle.
 */
   package sketcher;
   import java.awt.*;
   import java.awt.event.*;
   import java.awt.Cursor;
   import java.awt.Point;
   import java.awt.Graphics;
   import java.awt.Color;

   import com.sun.j3d.utils.universe.*;
   import com.sun.j3d.utils.geometry.*;
   import com.sun.j3d.utils.behaviors.mouse.*;
   import javax.media.j3d.*;
   import javax.vecmath.*;
   import javax.media.j3d.J3DGraphics2D;
   import com.sun.j3d.utils.image.TextureLoader;
   import com.sun.j3d.utils.behaviors.picking.*;
   import java.awt.image.BufferedImage;

   public class SKPointShape3d extends SKNormalShape3d
   {
      public  static SKMainFrame frameMain;
      public  J3DGraphics2D g= null;
      public  Transform3D sphere_transform = null, name_transform= null;
      public  Transform3D scale_transform = null;
      public  TransformGroup scale_group = null;
      public  TransformGroup sphere_group = null, name_group = null;
      public  Appearance ap1=null, ap=null;
      public  Sphere s =null;
      public  Color3f color = new Color3f(1.0f, 0.0f, 0.0f);
      public  double radius = 0.2;
      public Font    font    = new Font("SansSerif",Font.PLAIN,1);
      public Font3D  font3D  = new Font3D(font,new FontExtrusion());
      public Text3D   name ;
      public Shape3D nameShape ;


      public SKPointShape3d(SKMainFrame frame, int id, int x, int y, int z)
      {
         super(frame,id,"Point"+id,"Point",6);
         frameMain=frame;
         IconPath = "/images/pointicon.gif";
         //g = frameMain.panelShapeArea3d.canvas3D.getGraphics2D();
        
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
         s = new Sphere((float)0.2, Sphere.GENERATE_NORMALS|
                       Primitive.ENABLE_GEOMETRY_PICKING|
                       Primitive.ENABLE_APPEARANCE_MODIFY,
                       app);
         s.setCapability(Node.ENABLE_PICK_REPORTING);
         s.setUserData(this.Name);
         this.setUserData(this.Name);
	 name =  new Text3D(font3D, "P"+id);

	 Appearance appF = new Appearance();
         Color3f ambientColourF = new Color3f(0.2f, 0.2f, 0.2f);
         Color3f diffuseColourF = new Color3f(0.0f, 0.0f, 0.0f);
         Color3f specularColourF = new Color3f(1.0f, 1.0f, 1.0f);
         Color3f emissiveColourF = new Color3f(0.0f, 0.0f, 0.0f);
         Material matF = new Material(ambientColourF, emissiveColourF, diffuseColourF,
                           specularColourF, 0.5f);
	 matF.setShininess(70f);
         matF.setLightingEnable(true);
         appF.setMaterial(matF);
	 nameShape =  new Shape3D(name, appF);
	 //strName = "P"+id;
         if(frameMain.topView)
         {
            X= (x-442)/54.0f;
            Y=0;
            Z= -(368-y)/54.0f;
         }
         else
         {
            X= (x-442)/54.0f;
            Y= (368-y)/54.0f;
            Z=0;
         }
	 X=x;
         Y=y;
         Z=z;
         h=0;
         w=0;
      }
      public Node getAddTrans()
      {
	//name.setPosition(new Point3f((float) (X + 0.1), (float)Y, (float)Z));

         sphere_transform = new Transform3D();
         sphere_transform.setTranslation(new Vector3d((double)X,(double)Y,(double)Z));

	 name_transform = new Transform3D();
         name_transform.setTranslation(new Vector3d((double)X + radius + 0.1,(double)Y,(double)Z));
         adjustTransform(sphere_transform);
         scale_transform =  new Transform3D();
         scale_transform.setScale(new Vector3d(1.0,1.0,1.0));
	 scale_group = new TransformGroup();
         scale_group.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
         scale_group.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         scale_group.setTransform(scale_transform);
         scale_group.addChild(s);

         sphere_group = new TransformGroup();
	 name_group = new TransformGroup();
	 name_group.setTransform(name_transform);
	 name_group.addChild(nameShape);

         sphere_group.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
         sphere_group.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         sphere_group.setCapability(TransformGroup.ENABLE_PICK_REPORTING);
         sphere_group.setTransform( sphere_transform );
         sphere_group.addChild(scale_group);  // add a sphere
	 sphere_group.addChild(name_group);
         s.setUserData(this.Name);
	 sphere_group.setUserData(this.Name);
         //return sphere_group;
	 this.addChild(sphere_group);
	 return this;
      }

      public TransformGroup addPointGroup(float x, float y, float z)
      {
         sphere_transform = new Transform3D();
         sphere_transform.setTranslation(new Vector3d((double)x,(double)y,(double)z));
         adjustTransform(sphere_transform);
         sphere_group = new TransformGroup();
         sphere_group.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
         sphere_group.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         sphere_group.setCapability(TransformGroup.ENABLE_PICK_REPORTING);
         sphere_group.setTransform(sphere_transform);
         getMatrix();
         /*ap1 = new Appearance();
         ap1.setTextureUnitState(textureUnitState);
         Material mat = new Material();
         ap1.setMaterial(mat);*/
         ap1=setApp(1f,0f,0f);
         s = new Sphere((float)0.2, Sphere.GENERATE_NORMALS|
                       Primitive.ENABLE_GEOMETRY_PICKING|
                       Primitive.ENABLE_APPEARANCE_MODIFY,
                       ap1);
         s.setUserData(this.Name);
         sphere_group.addChild(s);  // add a sphere
         BoundingSphere bounds = new BoundingSphere(); // the render will only light objects inside this volume
         bounds.setRadius(1000.0);
         DirectionalLight lightD = new DirectionalLight();
         lightD.setInfluencingBounds(bounds);
         lightD.setDirection(new Vector3f(.7f, -.7f, -.7f));  // point the light right, down, into the screen
         frameMain.panelShapeArea3d.addLight(lightD);// add a light so we can see the sphere
         return sphere_group;
      }
/*      public void addPoint()
      {
         sphere_transform = new Transform3D();
         sphere_transform.setTranslation(new Vector3d((double)X,(double)Y,(double)Z));
         adjustTransform(sphere_transform);
         s.setUserData(this.Name);
         sphere_group = new TransformGroup();
         sphere_group.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
         sphere_group.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         sphere_group.setCapability(TransformGroup.ENABLE_PICK_REPORTING);
         sphere_group.setTransform( sphere_transform );
         sphere_group.addChild(s);  // add a sphere
      
         BoundingSphere bounds = new BoundingSphere(); // the render will only light objects inside this volume
         bounds.setRadius(100.0);
         Color3f lColor1 = new Color3f(1f, 1f, 1f);
      //         Vector3f lDir1  = new Vector3f(-1.0f, -1.0f, -1.0f);
         Vector3f lDir1 = new Vector3f(X,Y,Z);
         lDir1.negate();
         DirectionalLight lightD = new DirectionalLight(lColor1, lDir1);
         lightD.setInfluencingBounds(bounds);
         frameMain.panelShapeArea3d.addLight(lightD);// add a light so we can see the sphere
         frameMain.panelShapeArea3d.addObject(sphere_group, this.Name);
      }

      public void addPoint(JSelectionPanel3d panel)
	{
	 sphere_transform = new Transform3D();
         sphere_transform.setTranslation(new Vector3d((double)X,(double)Y,(double)Z));
         adjustTransform(sphere_transform);
         s.setUserData(this.Name);
         sphere_group = new TransformGroup();
         sphere_group.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
         sphere_group.setCapability(TransformGroup.ALLOW_TRANSFORM_READ); 
         sphere_group.setCapability(TransformGroup.ENABLE_PICK_REPORTING);
         sphere_group.setTransform( sphere_transform );
         sphere_group.addChild(s);  // add a sphere
         
         BoundingSphere bounds = new BoundingSphere(); // the render will only $
         bounds.setRadius(100.0);
         Color3f lColor1 = new Color3f(1f, 1f, 1f);
      //         Vector3f lDir1  = new Vector3f(-1.0f, -1.0f, -1.0f);
         Vector3f lDir1 = new Vector3f(X,Y,Z);
         lDir1.negate();
         DirectionalLight lightD = new DirectionalLight(lColor1, lDir1);
         lightD.setInfluencingBounds(bounds);
         panel.addLight(lightD);// add a light so we can s$
         panel.addObject(sphere_group, this.Name);
	}
*/
      public void getMatrix()
      {
         sphere_group.getTransform(sphere_transform);
         sphere_transform.get(matrix,vector);
         X=vector.x;
         Y=vector.y;
         Z=vector.z;
      }
      public void updateMainShapeData(SKPropArray vPropList, SKConstraintArray vCurrentConstraints)
      
      {
         super.updateMainShapeData(vPropList,vCurrentConstraints);
         if (vPropList != null)
         { //Update properties
            vPropList.add(frameMain.propFactory.getNewProp("Pt1 X",Double.toString(this.getDX()),1));
            vPropList.add(frameMain.propFactory.getNewProp("Pt1 Y",Double.toString(this.getDY()),1));
            vPropList.add(frameMain.propFactory.getNewProp("Pt1 Z",Double.toString(this.getDZ()),1));
         }
      }
   
      public void paintComponent(Graphics g)
      {}
   
      public int getNumSubShapes()
      {
         return 1;
      }
   
      public SKBaseShape3d getSubShape(int index)
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
      {}
      public void getTransform(Transform3D t)
      {sphere_group.getTransform(t);
      }
      public void doMove()
      {}
      public void doMove(Vector3f v)
      {
         //name.setString(strName);
         sphere_transform.setTranslation(v);
         sphere_group.setTransform( sphere_transform );
	 super.doMove(v);
      }
      public void doMove(Transform3D t)
      {
         //sphere_transform = t;
         //sphere_group.setTransform( sphere_transform );
      }
   
      public void doMoveOnPlane(int x, int y)
      {
         /*X= (x-442)/54.0f;
         Z=0f;
         if(frameMain.topView)
            Z= -(368-y)/54.0f;
         else
            Y= (368-y)/54.0f;
         Transform3D temp = new Transform3D();
         temp.setTranslation(new Vector3f(X,Y,Z));
         adjustTransform(temp);
         doMove(temp);
         getMatrix();*/
      }
   	
      public void changeRadius(double r)
      {
	 scale_transform =  new Transform3D();
         scale_transform.setScale(new Vector3d(r/0.2, r/0.2, r/0.2));
         scale_group.setTransform(scale_transform);
	 //radius = r;
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
         s.setAppearance(app);
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
	 s.setAppearance(app);
      }
	
      public void updateColors()
      {
	/******** UPDATE COLOR ************/
	Appearance app = new Appearance();
         Color3f ambientColour = new Color3f(0.2f, 0.2f, 0.2f);
         Color3f diffuseColour = new Color3f(1.0f, 0.0f, 0.0f);
         Color3f specularColour = new Color3f(1.0f, 1.0f, 1.0f);
         Color3f emissiveColour = new Color3f(0.0f, 0.0f, 0.0f);
         if(Selected)
         {
            Material mat = new Material(ambientColour, emissiveColour, diffuseColour,
                              specularColour, 0.5f);
            mat.setShininess(70f);
            mat.setLightingEnable(true);
            app.setMaterial(mat);
         }
         else
         {
            diffuseColour = new Color3f(0.0f, 1.0f, 0.0f);
            Material mat = new Material(ambientColour, emissiveColour, diffuseColour,
                              specularColour, 0.5f);
            mat.setShininess(70f);
            mat.setLightingEnable(true);
            app.setMaterial(mat);
         }
         s.setAppearance(app);

      }
   
   //Should this shape be drawn?
      public static Appearance setApp(float r, float g, float b)
      {
         Appearance app = new Appearance();
         app.setCapability(Appearance.ALLOW_COLORING_ATTRIBUTES_WRITE);
         Color3f objColor = new Color3f(r,g,b);
         ColoringAttributes ca = new ColoringAttributes();
         ca.setColor(objColor);
         app.setColoringAttributes(ca);
         return app;
      }
      public boolean shouldDraw()
      
      {
         //return (!frameMain.drawGroupRects || frameMain.selectedGroupShapes.indexOf(this) > -1);
         return true;
      }
      public int getShapeX()
      
      {
         return 0;
      
      }
   
   
      public int getShapeY()
      
      {
         return 0;}
      public void setShapePoint(Point3d pt)
      {
         doMove(new Vector3f((float)pt.x,(float)pt.y, (float)pt.z ));
      }
   
   }
