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

   import com.sun.j3d.utils.universe.*;
   import com.sun.j3d.utils.geometry.*;
   import com.sun.j3d.utils.behaviors.mouse.*;
   import javax.media.j3d.*;
   import javax.vecmath.*;
   import javax.media.j3d.J3DGraphics2D;
   import com.sun.j3d.utils.image.TextureLoader;
   import com.sun.j3d.utils.behaviors.picking.*;

   public class SKLineShape3d extends SKBaseShape3d
   {
      public static final String[] PtType  = new String[] {"Point","Infinite"};
      public static final String[] ConType = new String[] {"Arbitrary","Pt1","Pt2"};
      public double length = -1;
      public Point3d[] vertices = new Point3d[2];
      public Color3f[] colors = new Color3f[2];
      public Color3f[] colors1 = new Color3f[2];
      public Color3f color = new Color3f();
      public SKPointLineShape3d pt1 = null, pt2 = null;
      public LineArray    lineShape;
      public Cylinder     line;
      public TransformGroup cTrans, cScale, point1_group, point2_group, line_group;
      public TransformGroup group = new TransformGroup();
      public Transform3D cTranslate, cRotate, scaleTrans;
      public static SKMainFrame frameMain;

      public SKLineShape3d(SKMainFrame frame, int newID, int x, int y, int z)
      {
         super(frame,newID,"Line"+newID,"Line",7);
         frameMain=frame;
      //Create points
         pt1 = new SKPointLineShape3d(frame,newID+1,this,x-15,y);
         pt2 = new SKPointLineShape3d(frame,newID+2,this,x+15,y);
         colors[0]=new Color3f(1.0f,0.0f,0.0f);
         colors[1]=new Color3f(1.0f,0.0f,0.0f);
         colors1[0]=new Color3f(0.0f,1.0f,1.0f);
         colors1[1]=new Color3f(0.0f,1.0f,1.0f);
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


         line = new Cylinder(0.03f, 2f, Primitive.GENERATE_TEXTURE_COORDS |
                            Primitive.ENABLE_GEOMETRY_PICKING|
                            Primitive.ENABLE_APPEARANCE_MODIFY, app);
         line.setCapability(Primitive.ENABLE_APPEARANCE_MODIFY);
         line.setCapability(Node.ENABLE_PICK_REPORTING);
         line.setUserData("Line"+newID);
         if(frameMain.topView)
         {
            X= (x-442)/54.0f;
            Y=0;
            Z= -(368-y)/54.0f;
            addLine(X-1,0,Z,X+1,0,Z);
         }
         else
         {
            X= (x-442)/54.0f;
            Y= (368-y)/54.0f;
            Z=0;
            addLine(X-1,Y,0,X+1,Y,0);
         }

         IconPath = "/images/lineicon.gif";
         this.setUserData("Line"+newID);
      }
      public void addLine(float x1, float y1, float z1, float x2, float y2, float z2)
      {
         if(frameMain.topView)
         {point1_group=pt1.addPointGroup(x1,y1,z1);
            point2_group=pt2.addPointGroup(x2,y2,z2);}
         else
         { point1_group=pt1.addPointGroup(x1,y1,z1);
            point2_group=pt2.addPointGroup(x2,y2,z2);}
         vertices[0] = new Point3d((double)x1, (double)y1, (double)z1);
         vertices[1] = new Point3d((double)x2, (double)y2, (double)z2);
         adjustVertices(vertices);
         lineShape = new LineArray(2,GeometryArray.COORDINATES|GeometryArray.COLOR_3);
         lineShape.setCoordinates(0,vertices);
         lineShape.setColors(0,colors);
         lineShape.setCapability(LineArray.COLOR_3);
         lineShape.setCapability(LineArray.ALLOW_COLOR_WRITE);
         lineShape.setCapability(LineArray.ALLOW_COORDINATE_WRITE);
         lineShape.setCapability(Geometry.ALLOW_INTERSECT);
         Shape3D shape = new Shape3D(lineShape);
         line_group = new TransformGroup();
         shape.setCapability(Geometry.ALLOW_INTERSECT);
         line_group.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
         line_group.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         //line_group.setCapability(TransformGroup.ENABLE_PICK_REPORTING);
         line_group.addChild(shape);

         scaleTrans =  new Transform3D();
         scaleTrans.setScale(new Vector3d(1.0,1.0,1.0));
         cScale = new TransformGroup();
         cScale.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
         cScale.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         cScale.setTransform(scaleTrans);
         cScale.addChild(line);
         cTranslate =  new Transform3D();
         cRotate = new Transform3D();
         cTranslate.setTranslation(new Vector3f(X, Y, Z));
         adjustTransform(cTranslate);
         cRotate.setRotation(new AxisAngle4f(new Vector3f(0.0f, 0.0f, 1.0f), 1.57f));
         cTranslate.mul(cRotate);
         cTrans = new TransformGroup();
         cTrans.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
         cTrans.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         cTrans.setTransform(cTranslate);
         cTrans.addChild(cScale);

         group.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
         group.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
         //group.setCapability(TransformGroup.ENABLE_PICK_REPORTING);
         group.addChild(point1_group);
         group.addChild(point2_group);
         group.addChild(line_group);
         group.addChild(cTrans);
         frameMain.panelShapeArea3d.addObject(group,this.Name);

         pt1.getMatrix();
         pt2.getMatrix();
         length = (double)distance(new Point3f(pt1.vector.x, pt1.vector.y, pt1.vector.z), new Point3f(pt2.vector.x, pt2.vector.y, pt2.vector.z));
      }
      public void severTies()
      {
         super.severTies();
         pt1 = null;
         pt2 = null;
      }

      public void updateMainShapeData(SKPropArray vPropList, SKConstraintArray vCurrentConstraints)
      {
         super.updateMainShapeData( vPropList, vCurrentConstraints);

         if (vPropList != null)
         { //Update properties
            vPropList.add(frameMain.propFactory.getNewProp("Length", Double.toString(length),1,Color.red));
            vPropList.add(frameMain.propFactory.getNewComboProp("Pt1 Type",pt1.pointType,1,PtType) );
            vPropList.add(frameMain.propFactory.getNewProp("Pt1 X", Double.toString(pt1.getDX()),1) );
            vPropList.add(frameMain.propFactory.getNewProp("Pt1 Y", Double.toString(pt1.getDY()),1) );
            vPropList.add(frameMain.propFactory.getNewProp("Pt1 Z", Double.toString(pt1.getDZ()),1) );
            vPropList.add(frameMain.propFactory.getNewComboProp("Pt2 Type",pt2.pointType,1,PtType) );
            vPropList.add(frameMain.propFactory.getNewProp("Pt2 X", Double.toString(pt2.getDX()),1) );
            vPropList.add(frameMain.propFactory.getNewProp("Pt2 Y", Double.toString(pt2.getDY()),1) );
            vPropList.add(frameMain.propFactory.getNewProp("Pt2 Z", Double.toString(pt2.getDZ()),1) );
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
         /*         else if (PropName=="Pt1 X")
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
         }*/
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
         else
            return false;

      //Make panelShapeArea repaint
         frameMain.RefreshShapeArea();

         return true;
      }

      public void paintSpecial(Graphics g)
      {

         switch(pt1.pointType+pt2.pointType)
         {
            case 0:
               break;
            case 1:
               break;
            case 2:
               break;
         }
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
	 pt1.changeColor(c);
	 pt2.changeColor(c);
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
	 pt1.changeColor(c);
	 pt2.changeColor(c);
      }
      public void updateColors()
      {
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
            diffuseColour = new Color3f(0.0f, 0.0f, 1.0f);
            Material mat = new Material(ambientColour, emissiveColour, diffuseColour,
                              specularColour, 0.5f);
            mat.setShininess(70f);
            mat.setLightingEnable(true);
            app.setMaterial(mat);
         }
         line.setAppearance(app);
         pt1.updateColors();
         pt2.updateColors();      }

   //Should this shape be drawn?
      public boolean shouldDraw()
      {
         return true;//(!frameMain.drawGroupRects || frameMain.selectedGroupShapes.indexOf(this) > -1);
      }


      public void setSelected(boolean sel)
      {
         pt1.setSelected(sel);
         pt2.setSelected(sel);
         Selected = sel;
         updateColors();
      }

      public SKPointShape3d getOtherPoint(SKPointShape3d pt)
      {
         return (pt.equals(pt1)) ? pt2 : pt1;
      }

      public SKPointShape3d getLeftPoint()
      {
         return (pt1.getX()<pt2.getX()) ? pt1 : pt2;
      }

      public SKPointShape3d getTopPoint()
      {
         return (pt1.getY()<pt2.getY()) ? pt1 : pt2;
      }
      public int getConstraintMod(SKBaseShape3d sh)
      {
         if (sh == pt1)
            return 1;
         else if (sh == pt2)
            return 2;
         else if (sh == this)
            return 3;
         else
            return 0;
      }
      public SKPointShape3d getConstraintSubShape(SKBaseConstraint con, int mod)
      {
         SKPointShape3d p;
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
      public Point3d getConstraintPoint(SKBaseConstraint con, int mod)
      {
         Point3d pt = new Point3d();
         System.out.println("111 "+this+" "+mod);
         switch (mod)
         {
            case  1:
               { //Point 1
                  pt.x=pt1.getDX();
                  pt.y=pt1.getDY();
                  pt.z=pt1.getDZ();
               }
               break;
            case  2:
               { //Point 2
                  pt.x=pt2.getDX();
                  pt.y=pt2.getDY();
                  pt.z=pt2.getDZ();
               }
               break;
            case  3:
               { //line
                  System.out.println("111 do not draw");
               }
               break;
            default:
               { //Arbitrary
                  pt.x=(pt1.getDX()+pt2.getDX())/2;
                  pt.y=(pt1.getDY()+pt2.getDY())/2;
                  pt.z=(pt1.getDZ()+pt2.getDZ())/2;
                  if(con instanceof SKDistanceConstraint) ((SKDistanceConstraint)con).perp = true;
               }
               break;
         }

         return pt;
      }

      public void doMove(int deltaX, int deltaY, boolean resolveConstraints)
      {}
      public void doMove()
      {
         //System.out.println("111 do move");
         Vector3f v1 = new Vector3f();
         Vector3f v2 = new Vector3f();
         Vector3f v3 = new Vector3f();
         pt1.getMatrix();
         v1 = pt1.vector;
         vertices[0] = new Point3d((double)pt1.vector.x, (double)pt1.vector.y, (double)pt1.vector.z);
         pt2.getMatrix();
         v2 = pt2.vector;
         v3 = pt2.vector;
         vertices[1] = new Point3d((double)pt2.vector.x, (double)pt2.vector.y, (double)pt2.vector.z);
         lineShape.setCoordinates(0,vertices);

         double newLength = (double)distance(new Point3f(pt1.vector.x, pt1.vector.y, pt1.vector.z), new Point3f(pt2.vector.x, pt2.vector.y, pt2.vector.z));
         v2.x-=v1.x;
         v2.y-=v1.y;
         v2.z-=v1.z;
         normalize(v2);
         float dProduct = dotProduct(new Vector3f(0f,1.0f,0f), v2);
         double angle = java.lang.Math.acos(dProduct);
         Vector3f cProduct = crossProduct(new Vector3f(0f,1.0f,0f), v2);
         normalize(cProduct);
         cRotate = new Transform3D();
         cRotate.setRotation(new AxisAngle4f(cProduct,(float)angle));
         double newScale = newLength/length;
         float moveScale = (float) ((newLength - length)/2);
         scaleTrans =  new Transform3D();
         scaleTrans.setScale(new Vector3d(1.0,newScale,1.0));
         Vector3f translate =  new Vector3f(v1.x+v3.x/2, v1.y+v3.y/2 , v1.z+v3.z/2);
         cTranslate = new Transform3D();
         cTranslate.setTranslation(translate);

         Vector3f translate1 =  new Vector3f(0f, moveScale +0.5f, 0);
         Transform3D cTranslate1 = new Transform3D();
         cTranslate1.setTranslation(translate1);

         cScale.setTransform(scaleTrans);
         cRotate.mul(cTranslate1);
         cTranslate.mul(cRotate);
         cTrans.setTransform(cTranslate);
      }
      public static float magnitude(Vector3f v)
      {
         float answer = v.x*v.x + v.y*v.y + v.z*v.z;
         answer = (float)java.lang.Math.sqrt((double)answer);
         return answer;
      }
      public static void normalize(Vector3f v)
      {
         float mag = magnitude(v);
         v.x/=mag;
         v.y/=mag;
         v.z/=mag;
      }
      public static Vector3f crossProduct(Vector3f v1, Vector3f v2)
      {
         Vector3f answer = new Vector3f();
         answer.x = v1.y*v2.z - v1.z*v2.y;
         answer.y = v1.z*v2.x - v1.x*v2.z;
         answer.z = v1.x*v2.y - v1.y*v2.x;
         return answer;
      }
      public static float dotProduct(Vector3f v1, Vector3f v2)
      {
         float answer;
         answer = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
         return answer;
      }
      public boolean doHitTest(int x, int y)
      {
         return false;
      }

   /*public int getNumConstraintPoints(SKBaseConstraint con)
      {
         return 2;
      }
   */
      public int getNumSubShapes()
      {
         return 2;
      }

      public SKBaseShape3d getSubShape(int index)
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

      public void readAdditionalProps(DataInputStream p) throws IOException
      {
         super.readAdditionalProps(p);

         length = p.readDouble();
         pt1.readAdditionalProps(p);
         pt2.readAdditionalProps(p);
      }

      public boolean isPrimaryShape(SKBaseShape3d sh)
      {
         return (pt1==sh);
      }

      public int getShapeX()
      {
         return 0;
      }

      public int getShapeY()
      {
         return 0;}
      public static float distance(Point3f p1, Point3f p2)
      {
         float answer = (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) + (p1.z-p2.z)*(p1.z-p2.z);
         answer = (float) java.lang.Math.sqrt((double)answer);
         return answer;
      }
      public static double distance(Point3d p1, Point3d p2)
      {
         double answer = (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) + (p1.z-p2.z)*(p1.z-p2.z);
         answer = java.lang.Math.sqrt(answer);
         return answer;
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
     public int bifurUpdate(float[] data, int startIdx, int addX, int addY, float scale)
      {
         startIdx = pt1.bifurUpdate(data,startIdx,addX,addY,scale);
         startIdx = pt2.bifurUpdate(data,startIdx,addX,addY,scale);
         return startIdx+1;
      }
     public int bifurDataCount()
      {
         return pt1.bifurDataCount() + pt2.bifurDataCount() ;
      }

   }
