/**
 * Title:        SKUTU
 * Description:  Sketcher's Universal Translation Unit abstract class.  All UTUs should descend from this class
 */
   package sketcher;

   public abstract class SKUTU
   {
      public abstract void solveSystem(SKMainFrame frameMain, SKShapeArray shapes, SKConstraintArray cons);
   }