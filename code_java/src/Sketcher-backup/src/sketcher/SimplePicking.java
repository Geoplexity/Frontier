   package sketcher;

   import javax.media.j3d.*;
   import com.sun.j3d.utils.behaviors.picking.*;
   //import com.sun.j3d.utils.picking.behaviors.PickMouseBehavior;

   public class SimplePicking extends PickMouseBehavior {
      protected SimplePickingCallback callback = null;
      protected int pickMode = PickObject.USE_BOUNDS;
      protected int nodeType = PickObject.SHAPE3D;
      protected Node pickedNode = null;
   
      public SimplePicking(BranchGroup root, Canvas3D canvas, Bounds bounds) {
         super(canvas, root, bounds);
      
         this.setSchedulingBounds(bounds);
      }
   
      public SimplePicking( BranchGroup root, Canvas3D canvas, Bounds bounds,
                        int mode )
      {
         super(canvas, root, bounds);
         pickMode = mode;
      
         this.setSchedulingBounds(bounds);
      }
   
      public SimplePicking( BranchGroup root, Canvas3D canvas, Bounds bounds,
                        int mode, int type)
      {
         super(canvas, root, bounds);
         pickMode = mode;
         nodeType = type;
      
         this.setSchedulingBounds(bounds);
      }
   
      public void setPickMode(int mode) { 
         pickMode = mode; }
      public int getPickMode() { 
         return pickMode; }
   
      public void setNodeType(int type) { 
         nodeType = type; }
      public int getNodeType() { 
         return nodeType; }
   
      public void setupCallback(SimplePickingCallback callback) { 
         this.callback = callback; }
   
      public void updateScene(int x, int y) {
         Node node = pickScene.pickNode(pickScene.pickClosest(x, y), nodeType);
         callback.picked(nodeType, node);
         pickedNode = node;
         callback.handlePicked(node);
      }
   
      public Node getNode()
      {
         return pickedNode;
      }
   }
