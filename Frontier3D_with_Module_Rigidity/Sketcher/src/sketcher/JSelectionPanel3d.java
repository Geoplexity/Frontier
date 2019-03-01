
/**
 * Title:        JSelectionPanel
 * Description:  Extends a JPanel to allow for drawing a selection rect
 */
   package sketcher;
   import javax.swing.JPanel;
   import java.awt.Graphics;
   import java.awt.Graphics2D;
   import java.awt.Stroke;
   import java.awt.BasicStroke;
   import java.awt.Color;
   import java.awt.*;
   import java.awt.event.*;
   import java.awt.geom.Rectangle2D;

   import com.sun.j3d.utils.universe.*;
   import com.sun.j3d.utils.geometry.*;
   import com.sun.j3d.utils.behaviors.mouse.*;
   import javax.media.j3d.*;
   import javax.vecmath.*;
   import javax.media.j3d.J3DGraphics2D;
   import com.sun.j3d.utils.behaviors.picking.*;
   import com.sun.j3d.utils.behaviors.mouse.MouseTranslate;

   public class JSelectionPanel3d extends JPanel
   {
      public SKMainFrame frameMain;
      public boolean DrawSelRect = false;
      public boolean DrawConstraints = true;
      public boolean moveWorld=true;
      public boolean showAxes=true;
      public boolean moveInPlane=false;
      public boolean reset=false;
      public boolean control=false;
      public boolean axis3d=false;
      public int     StartX,StartY,EndX,EndY;
      public float   scale=1;
      public Canvas3D canvas3D ;
   
      public Background  bg= new Background( new Color3f(0.83f,0.83f,0.83f));
   
      public BranchGroup scene = new BranchGroup();
      public BranchGroup viewBranch = new BranchGroup();
      public BranchGroup topViewBranch = new BranchGroup();
      public BranchGroup axisGroup = new BranchGroup();
      public BranchGroup axis3dGroup = new BranchGroup();
   
      public TransformGroup root_group = new TransformGroup();
      public TransformGroup vpTransGr, axis_group, axis_group3d;
   
      public Transform3D t = new Transform3D();
      public Transform3D savedT = new Transform3D();
      public Transform3D savedTAxis = new Transform3D();
      public Transform3D savedTObjects = new Transform3D();
   
      public MouseTranslate mouseMove = new MouseTranslate();
      public MouseRotate mouseRotate = null;
      public MouseRotate mouseRotateAxis = null;
   
      public SimpleTranslating translator;
      public SimpleZooming zoomer;
   
      public MouseRotate mouseRotateAxis3d = null;
      public VirtualUniverse virtualU=null;
      public Locale locale=null;
      public BoundingSphere bounds;
      public SKBaseShape pi = null;
   
      static final byte X = 0;
      static final byte Y = 1;
      static final byte Z = 2;
      public JSelectionPanel3d(SKMainFrame mainFrame)
      {
         super();
         frameMain = mainFrame;
         canvas3D = new Canvas3D( frameMain.getGraphicsConfiguration() );
         //System.out.println(canvas3D.getGraphicsContext3D());
         virtualU = new VirtualUniverse();
         locale= new Locale(virtualU);
         canvas3D.addMouseListener(
                              
                                 new MouseListener(){
                                    public void mouseClicked(MouseEvent e)
                                    {//System.out.println("111 mouse clicked at : "+e.getX()+" "+e.getY());
                                    }
                                    public void mousePressed(MouseEvent e) {
                                       if (e.isControlDown())
                                          control = true;
                                       else
                                          control = false;
                                    }
                                    public void mouseReleased(MouseEvent e) {
                                    }
                                    public void mouseEntered(MouseEvent e) {
                                    }
                                    public void mouseExited(MouseEvent e) {
                                    }
                                 });
         canvas3D.addMouseMotionListener(
                              
                                 new MouseMotionListener(){
                                    public void mouseDragged(MouseEvent e)
                                    {
                                       for(int i=0; i<frameMain.allshapes3d.size(); i++)
                                       {
                                          SKBaseShape3d sh = frameMain.allshapes3d.get(i);
                                          for(int j=0; j<sh.getNumSubShapes()+1; j++)
                                          {
                                             SKBaseShape3d sh1 = sh.getSubShape(j);
                                             if(sh1.clicked)
                                             {
                                                //System.out.println("111 mouse dragged at "+e.getX()+" "+e.getY());
                                                sh1.doMoveOnPlane(e.getX(), e.getY());
                                             }
                                          }
                                       	//if(sh.DragState==0)
                                          if(sh.Selected)
                                          {
                                             sh.doMove();
                                             sh.getSelectable().updateMainShapeData(frameMain.vCurrentProps,null);
                                             for(int j=0; j<sh.ConstraintList.size(); j++)
                                             {
                                                SKBaseConstraint3d con = sh.ConstraintList.get(j);
                                             }
                                             frameMain.tableObjectProp.updateUI();
                                             sh.DragState=0;
                                          }
                                          else
                                             sh.DragState++;
                                       }
                                       if(moveWorld)
                                       {
					  //mouseRotate.setEnable(true);
                                          //mouseRotateAxis.setEnable(true);

                                          if(reset)
                                          {
                                             reset = false;
                                             Transform3D trans = new Transform3D();
                                             mouseRotate.transformChanged(trans);
                                          }
                                       }
                                    }
                                 
                                    public void mouseMoved(MouseEvent e)
                                    {}
                                 });
      
         viewBranch.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE); // allow the mouse behavior to rotate the scene
         viewBranch.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         viewBranch.setCapability(BranchGroup.ALLOW_DETACH);
         ViewPlatform viewPlatform = new ViewPlatform();
         viewPlatform.setActivationRadius(Float.MAX_VALUE);
         bounds = new BoundingSphere(new Point3d(0.0,0.0,0.0), Double.MAX_VALUE);
         viewPlatform.setBounds(bounds);
         t.set(new Vector3f(0.0f, 2.0f, 25.0f));
         vpTransGr = new TransformGroup(t);
         vpTransGr.addChild(viewPlatform);
         viewBranch.addChild(vpTransGr);
         View view = new View();
         view.setProjectionPolicy(View.PERSPECTIVE_PROJECTION );
         view.addCanvas3D(canvas3D);
         PhysicalBody body = new PhysicalBody();
         view.setPhysicalBody(body);
         PhysicalEnvironment environment = new PhysicalEnvironment();
         view.setPhysicalEnvironment(environment);
         view.attachViewPlatform(viewPlatform);
      
         locale.addBranchGraph(viewBranch);
      
         root_group.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
         root_group.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         root_group.setCapability(TransformGroup.ALLOW_CHILDREN_READ);
      
         axisGroup.setCapability(TransformGroup.ALLOW_CHILDREN_READ);
         axis3dGroup.setCapability(TransformGroup.ALLOW_CHILDREN_READ);
      
         scene.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
         scene.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         scene.setCapability(TransformGroup.ALLOW_CHILDREN_READ);
         scene.setCapability(TransformGroup.ALLOW_CHILDREN_WRITE);
         scene.addChild(root_group);
         bg.setApplicationBounds(bounds);
         scene.addChild(bg);
         createSceneGraph( virtualU,scene,root_group,1 );
      
         this.setLayout( new BorderLayout() );
         this.setOpaque( false );
         this.add("Center", canvas3D);
      
      }
      public void viewTop()
      {
         viewBranch.detach();
         savedT = new Transform3D();
         vpTransGr.getTransform(savedT);
         savedTAxis = new Transform3D();
         axis_group.getTransform(savedTAxis);
         savedTObjects = new Transform3D();
         root_group.getTransform(savedTObjects);
         t.setIdentity();
         root_group.setTransform(t);
         axis_group.setTransform(t);
      
         t.set(new Vector3f(0.0f, 20.0f, 0.0f));
         Transform3D t1 = new Transform3D();
         t1.rotX((Math.PI/180)*-90);
         t.mul(t1);
         vpTransGr.setTransform(t);
         locale.addBranchGraph(viewBranch);
      }
      public void viewFront()
      {
         viewBranch.detach();
         savedT = new Transform3D();
         vpTransGr.getTransform(savedT);
         savedTAxis = new Transform3D();
         axis_group.getTransform(savedTAxis);
         savedTObjects = new Transform3D();
         root_group.getTransform(savedTObjects);
         t.setIdentity();
         root_group.setTransform(t);
         axis_group.setTransform(t);
      
         t.set(new Vector3f(0.0f, 0.0f, 20.0f));
         vpTransGr.setTransform(t);
         locale.addBranchGraph(viewBranch);
      }
      public void viewDefault()
      
      {
         viewBranch.detach();
         t=new Transform3D(savedT);
         vpTransGr.setTransform(t);
         axis_group.setTransform(savedTAxis);
         root_group.setTransform(savedTObjects);
         locale.addBranchGraph(viewBranch);
      }
      public void resetView()
      {
         if(reset)
         {
            viewBranch.detach();
            t.setIdentity();
            root_group.setTransform(t);
            axis_group.setTransform(t);
            if(axis3d)
               axis_group3d.setTransform(t);
            t.set(new Vector3f(0.0f, 2.0f, 25.0f));
            vpTransGr.setTransform(t);
            locale.addBranchGraph(viewBranch);
         }
         else
         {
         }
      }
      public Transform3D getView()
      {
         return t;
      }
      public Transform3D getRotation()
      {
         Transform3D rotation = new Transform3D();
         root_group.getTransform(rotation);
         return rotation;
      }
      public void paint(Graphics g)
      {
         super.paint(g);
      }
      public void paintComponent(Graphics g)
      {
         super.paintComponent(g);
      
      }
      void createSceneGraph(VirtualUniverse virtualU,BranchGroup objRoot, TransformGroup root_group, int scene_type )
      {
         mouseRotate = new MouseRotate( root_group );  // add the mouse behavior
         mouseRotate.setSchedulingBounds( new BoundingSphere() );
         scene.addChild(mouseRotate);
         createAxis(virtualU, axisGroup);
      
         /*PickTranslateBehavior zoomer =
         new PickTranslateBehavior(scene, canvas3D, bounds, PickObject.USE_GEOMETRY);*/
         translator = new SimpleTranslating(scene, canvas3D, bounds, PickObject.USE_GEOMETRY);
         translator.setupCallback(
                                 new SimpleTranslatingCallback()
                                 {
                                    public void transformChanged(int type, Transform3D transform)
                                    {
                                    }
                                 });
         //scene.addChild(translator);
      
         /*PickZoomBehavior zoomer =
         new PickZoomBehavior(scene, canvas3D, bounds, PickObject.USE_GEOMETRY);*/
         zoomer = new SimpleZooming(scene, canvas3D, bounds, PickObject.USE_GEOMETRY);
         zoomer.setupCallback(
                             
                                new SimpleZoomingCallback()
                                
                                {
                                
                                   public void transformChanged(int type, Transform3D transform)
                                   {
                                   }
                                });
         scene.addChild(zoomer);
      
         SimplePicking picker =
         new SimplePicking( scene, canvas3D, bounds,
                          PickObject.USE_GEOMETRY, PickObject.PRIMITIVE );
         SimplePickingCallback pick =
            new SimplePickingCallback() {
            
               public void picked(int type, Node node) {
	  	  System.out.println("111 move "+node+" move "+moveWorld);
                  if (node != null)
                  {
/********* testing
	             moveWorld=false;
                     mouseRotate.setEnable(false);
                     mouseRotateAxis.setEnable(false);    ************/
                     //mouseRotateAxis3d.setEnable(false);
                  }
                  else
                  {
                     if(!moveWorld)
                     {
                        mouseRotate.setEnable(true);
                        mouseRotateAxis.setEnable(true);
                        //mouseRotateAxis3d.setEnable(true);
                        moveWorld=true;
                     }
                     for(int i=0; i<frameMain.allshapes.size();i++)
                        frameMain.allshapes.get(i).Selected=false;
                  }
               }
               public void handlePicked(Node node)
               {
                  if(node!=null)
                  {
                     String pickName = (String)node.getUserData();
                  
                     SKBaseShape3d picked = getSelectable(pickName, 1);
                     if(picked!=null)
		     {
                     for(int i=0; i<frameMain.allshapes3d.size(); i++)
                     {	SKBaseShape3d sh = frameMain.allshapes3d.get(i);
                        for(int j=0; j<sh.getNumSubShapes()+1;j++)
                        {SKBaseShape3d sh1=sh.getSubShape(j);
                           if(!control)
                              sh1.clicked=false;}
                     }
                     picked.clicked = true;
                     SKBaseShape3d pickedShape = getSelectable(pickName, 0);
                     if(pickedShape != picked)
                        pickedShape.selShape=picked;
                     else if(pickedShape instanceof SKLineShape3d)
                        pickedShape.selShape = picked;
                     //System.out.println("111 >> "+pickedShape+" "+picked+" "+pickedShape.selShape);
                  
                     for(int i=0;i<frameMain.allshapes.size(); i++)
                     {
                        SKBaseShape sh=frameMain.allshapes.get(i);
                     
                        if(sh.Name!=pickedShape.Name)
                        {
                           if(!control)
                           {
                              sh.setSelected(false);
                              frameMain.SelectedShapes.removeShape(sh);
                           }
                        
                        }
                        else
                        {
                           frameMain.mouseOverItem = sh;
                           sh.getSelectable().updateMainShapeData(frameMain.vCurrentProps,null);
                           frameMain.cmbShapes.setSelectedItem(sh.getSelectable());
                        
                           frameMain.tableObjectProp.updateUI();
                           if (control)
                           { frameMain.toggleSelectedShape(sh);
                           }
                           else
                           {
                              pi=sh;
                              sh.setSelected(true);
                              sh.DragState=0;
                              frameMain.SelectedShapes.add(sh);
                           
                           }
                        /*
                           if(sh instanceof SKPlaneShape)
                              ((SKPlaneShape)sh).setDragType(pickName);
                           else if(sh instanceof SKSphereShape)
                              ((SKSphereShape)sh).setDragType(pickName);
                           else if(sh instanceof SKCylinderShape)
                              ((SKCylinderShape)sh).setDragType(pickName);
                           else if(sh instanceof SKConeShape)
                              ((SKConeShape)sh).setDragType(pickName);
                        */
                        }
                     }
		     }
                  }
                  else
                  {
                     frameMain.mouseOverItem = null;
                     for(int i=0;i<frameMain.allshapes.size(); i++)
                     {
                        SKBaseShape sh=frameMain.allshapes.get(i);
                        sh.setSelected(false);
                        frameMain.SelectedShapes.removeShape(sh);
                     }
                  }
               }
            };
         picker.setupCallback( pick);
         scene.addChild(picker);
         locale.addBranchGraph(scene);
      
         Color3f lightColor = new Color3f(1.0f, 1.0f, 1.0f);
         Vector3f lightDirectionFront  = new Vector3f(0f,0f,-1f);
         Vector3f lightDirectionTop  = new Vector3f(0f,-1f,0f);
         DirectionalLight dirLightFront = new DirectionalLight(lightColor, lightDirectionFront);
         DirectionalLight dirLightTop = new DirectionalLight(lightColor, lightDirectionTop);
         dirLightFront.setInfluencingBounds(bounds);
         dirLightTop.setInfluencingBounds(bounds);
         addLight(dirLightFront);
         addLight(dirLightTop);
      }
      SKBaseShape3d getSelectable(String name, int mode)
      {
      
         for(int i=0; i<frameMain.allshapes3d.size(); i++)
         {	SKBaseShape3d sh = frameMain.allshapes3d.get(i);
         
            for(int j=0; j<sh.getNumSubShapes()+1;j++)
            {
               SKBaseShape3d sh1=sh.getSubShape(j);
            
               if(name.equals(sh1.Name))
               {
                  if(mode ==0)
                     return sh1.getSelectable();
                  else
                     return sh1;
               }
            }
         }
         return null;
      }
      void createAxis(VirtualUniverse virtualU, BranchGroup objRoot)
      
      {
         objRoot.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE); // allow the mouse behavior to rotate the scene
         objRoot.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         axis_group = new TransformGroup();
         axis_group.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE); // allow the mouse behavior to rotate the scene
         axis_group.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         objRoot.addChild(axis_group);
         Transform3D obj_trans3Da = new Transform3D();
         axis_group.setTransform(obj_trans3Da);
      
         Appearance app = setApp(1,0,0);
         int z=-25;
         int x=-25;
         for(int i=0;i<51;i++)
         {
            app=setApp(1,1,1);
            axis_group.addChild(new Axis(X,app,z));
            axis_group.addChild(new Axis(Z,app,x));
            z++;
            x++;
         }
         app = setApp(1,1,0);
         NewShapes axisx = new Axis(X,app,0);
         app = setApp(0,1,0);
         NewShapes axisy = new Axis(Y,app,0);
         app = setApp(0,0,1);
         NewShapes axisz = new Axis(Z,app,0);
      
         axis_group.addChild(axisx);
         axis_group.addChild(axisy);
         axis_group.addChild(axisz);
         if(moveWorld)
         {
            mouseRotateAxis = new MouseRotate( axis_group);  // add the mouse behavior
            mouseRotateAxis.setSchedulingBounds( new BoundingSphere() );
            objRoot.addChild( mouseRotateAxis);
         }
         else
         {
            objRoot.removeChild(1);
         }
         if(!showAxes)
            objRoot.detach();
         locale.addBranchGraph(objRoot);
      }
      void create3dAxis(VirtualUniverse virtualU, BranchGroup objRoot, TransformGroup axis3d_group)
      {
         objRoot.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE); // allow the mouse behavior to rotate the scene
         objRoot.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         axis3d_group = new TransformGroup();
         axis3d_group.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE); // allow the mouse behavior to rotate the scene
         axis3d_group.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         objRoot.addChild(axis3d_group);
         Transform3D obj_trans3Da = new Transform3D();
         axis3d_group.setTransform(obj_trans3Da);
         Appearance app = setApp(1,0,0);
         int z=0,z1=0;
         int y=0,y1=0;
         int x=0,x1=0;
         for(int i=0;i<11;i++)
            for(int j=0;j<11;j++)
            {
               app=setApp(1,0,1);
               axis3d_group.addChild(new Axis3d(Z,app,i,j));
               axis3d_group.addChild(new Axis3d(Y,app,i,j));
               axis3d_group.addChild(new Axis3d(X,app,i,j));
            }
         if(moveWorld)
         {
            mouseRotateAxis3d = new MouseRotate( axis3d_group);  // add the mouse behavior
            mouseRotateAxis3d.setSchedulingBounds( new BoundingSphere() );
            objRoot.addChild( mouseRotateAxis3d);
         }
         else
         {
            objRoot.removeChild(1);
         }
         if(!showAxes)
            objRoot.detach();
         locale.addBranchGraph(objRoot);
      }
      public static Appearance setApp(float r, float g, float b)
      
      {
         Appearance app = new Appearance();
         Color3f objColor = new Color3f(r,g,b);
         ColoringAttributes ca = new ColoringAttributes();
         ca.setColor(objColor);
         app.setColoringAttributes(ca);
         PolygonAttributes pa = new PolygonAttributes();
         pa.setPolygonMode(pa.POLYGON_LINE);
         pa.setCullFace(pa.CULL_NONE);
         app.setPolygonAttributes(pa);
         return app;
      }
      void addObject(Node cube_group)
      {
         scene.detach();
         root_group.addChild( cube_group);
         locale.addBranchGraph(scene);
      }
   
      void addObject(TransformGroup cube_group, String data)
      {
         scene.detach();
         cube_group.setUserData(data);
         root_group.addChild( cube_group);
         locale.addBranchGraph(scene);
      }
      void addLight(DirectionalLight light)
      {
         scene.detach();
         scene.addChild( light);
         locale.addBranchGraph(scene);
      }
      void remove(SKBaseShape sh)
      {
      
      }
      public SKBaseShape getObject(int i)
      {
         SKBaseShape sh=null;
         return sh;
      }
      public int getComponentCount()
      {
         return -1;
      }
      public void add(SKBaseConstraint3d con)
      {
	addObject(con.getAddTrans());
      }	
      public void add(SKBaseShape3d sh)
      {
	sh.getAddTrans();
	addObject(sh);
      }
   }
