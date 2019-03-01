/**
 * Title:        SKBaseIncidenceConstraint
 * Description:  Incidence Constraint
 */
   package sketcher;

   public class SKIncidenceConstraint extends SKNormalConstraint
   {
      public SKIncidenceConstraint(SKMainFrame frameMain, int idnum)
      {
         super(frameMain,idnum,"Incidence"+idnum,"Incidence",1);
      }
   
   }