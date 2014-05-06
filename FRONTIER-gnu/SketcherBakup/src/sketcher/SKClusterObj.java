   // Changed to accomodate the new additions we are making....

   package sketcher;

   import java.awt.*;
   import java.awt.event.*;


   public class SKClusterObj
   {
      int ID= -1;
      int Int = -1;
      int Dbl = -1;
      int index = -1;
   	int bifurChoice = -1;
	int selectBifur = -1; // added which indicates the bifurcation choice.
      public SKClusterObj(int clusterID, int indInt, int indDbl, int numCluster, int c)
      {
         ID = clusterID;
         Int = indInt;
         Dbl = indDbl;
         index  = numCluster;
      	bifurChoice = c;
      }
   
   }
