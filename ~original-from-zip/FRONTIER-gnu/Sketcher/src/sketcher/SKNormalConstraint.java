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

   public class SKNormalConstraint extends SKBaseConstraint
   {
      public int    config1=1,
      config2=1; //Configs for associated shapes
      public SKNormalConstraint(SKMainFrame frameMain, int idnum, String name, String type, int typeID)
      {
         super(frameMain,idnum,name,type,typeID);
      }
   
      public int getConInfo(SKBaseShape sh, int code)
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
            if (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0)
            {
               if(SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] == 0)
                  SKSimpleSolver.ResolveSimpleConstraints(ShapeList.get(0),new SKConstraintArray(3));
               else 
                  SKSimpleSolver.ResolveSimpleConstraints(ShapeList.get(0));
            }
         }
         else if (ShapeList.get(1).Name==PropName)
         {
            config2 = ShapeList.get(1).getConPropInfo(this,PropName,PropData);
            if (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0)
            {
               if(SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] == 0)
                  SKSimpleSolver.ResolveSimpleConstraints(ShapeList.get(1),new SKConstraintArray(3));
               else 
                  SKSimpleSolver.ResolveSimpleConstraints(ShapeList.get(1));
            }
         }
         else
            return false;
         frameMain.RefreshShapeArea();
      
         return true;
      }
   
      public void doPaint(Graphics g)
      {
         super.doPaint(g);
      }
      public void drawConstraint(Graphics g, int x1,int y1, int x2, int y2)
      {
         super.drawConstraint(g,x1,y1,x2,y2);
      }
      public void addShape(SKBaseShape sh)
      {
         super.addShape(sh);
      
         int idx = ShapeList.indexOf(sh);
         if (sh.selShape != sh)
            if (idx == 0)
               config1 = sh.getConstraintMod(sh.selShape);
            else
               config2 = sh.getConstraintMod(sh.selShape);
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