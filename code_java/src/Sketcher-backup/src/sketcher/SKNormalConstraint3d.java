/**
 * Title:        SKNormalConstraint
 * Description:  Ancestor class for all 2-shape constraints (Incidence,Perpendicular,Parallel,etc)
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Color;
   import java.awt.Point;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;

   public class SKNormalConstraint3d extends SKBaseConstraint3d
   {
      public int    config1=1,
      config2=1, config3=1;
      public SKNormalConstraint3d(SKMainFrame frameMain, int idnum, String name, String type, int typeID)
      {
         super(frameMain,idnum,name,type,typeID);
      }

      public int getConInfo(SKBaseShape3d sh, int code)
      {
         if (sh==ShapeList.get(0))
            return config1;
         else
            return config2;
      }

      public boolean setMainData(String PropName, Object PropData)
      {
         if (super.setMainData(PropName,PropData))
            return true;
         if (ShapeList.get(0).Name==PropName)
         {
            config1 = ShapeList.get(0).getConPropInfo(this,PropName,PropData);

         }
         else if (ShapeList.get(1).Name==PropName)
         {
            config2 = ShapeList.get(1).getConPropInfo(this,PropName,PropData);

         }
         else if(ShapeList.size()==3)
	 {
	   if (ShapeList.get(2).Name==PropName)
	    config3 = ShapeList.get(2).getConPropInfo(this,PropName,PropData);
	 }
	 else
            return false;
         frameMain.RefreshShapeArea();

         return true;
      }

      public void doPaint()
      {
         super.doPaint();
      }
      public void drawConstraint(double x1,double y1, double z1,double x2, double y2, double z2)
      {
         super.drawConstraint(x1,y1,z1,x2,y2,z2);
      }
      public void addShape(SKBaseShape sh)
      {
         super.addShape(sh);
         SKBaseShape3d s=null;
	 for(int i=0; i<frameMain.allshapes3d.size(); i++)
	    if(sh.ID == frameMain.allshapes3d.get(i).ID)
		s = frameMain.allshapes3d.get(i);
         int idx = ShapeList.indexOf(s);
         if ((sh.selShape != sh) || (s instanceof SKLineShape3d))
         {
  	    if (idx == 0)
               config1 = s.getConstraintMod(s.selShape);
            else if(idx == 1)
               config2 = s.getConstraintMod(s.selShape);
            else
               config3 = s.getConstraintMod(s.selShape);
	 }
      }

      public boolean constrEquals(SKShapeArray selectedShapes, SKBaseConstraint con)
      { //Since the user can change the constraint points with a shape with SubShapes, you cannot say any SKNormalConstraints will remain equal
         return false;
      }

      public void writeAdditionalProps(DataOutputStream p) throws IOException
      {
         super.writeAdditionalProps(p);

         p.writeInt( config1 );
         p.writeInt( config2 );
      }

      public void readAdditionalProps(DataInputStream p) throws IOException
      {
         super.readAdditionalProps(p);

         config1 = p.readInt();
         config2 = p.readInt();
      }
   }
