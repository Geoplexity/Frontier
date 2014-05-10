/**
 * Title:        SKTangentConstraint
 * Description:  Represents tangent Line-Circle constraints
 */
   package sketcher;

   public class SKTangentConstraint extends SKBaseConstraint
   {
   
      public SKTangentConstraint(SKMainFrame frameMain, int idnum)
      {
         super(frameMain,idnum,"Tangent"+idnum,"Tangent",5);
      
         autoAddShapeProps = false;
         drawMode = 1;
      }
   
      public boolean setMainData(String PropName, Object PropData)
      {
         return super.setMainData(PropName,PropData);
      }
   
      public static boolean isAvailable(SKShapeArray selectedShapes)
      {
         return ( (selectedShapes.get(0) instanceof SKLineShape) && (selectedShapes.get(1) instanceof SKCircleShape) ||
                   (selectedShapes.get(0) instanceof SKCircleShape) && (selectedShapes.get(1) instanceof SKLineShape) ||
                   (selectedShapes.get(0) instanceof SKCircleShape) && (selectedShapes.get(1) instanceof SKCircleShape)
                );
      }
   }