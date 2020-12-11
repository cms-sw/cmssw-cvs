// @(#)root/eve:$Id: geom_core_parts.C,v 1.1 2010/03/23 19:30:47 matevz Exp $
// Author: Matevz Tadel

// Shows CMS tracker, calo and muon system.
// Depth can be set for each part independently.

void geom_core_parts()
{
   TEveUtil::LoadMacro("common_foos.C");
   std_init();

   make_node("/cms:World_1/cms:CMSE_1/tracker:Tracker_1", 4, kTRUE);
   make_node("/cms:World_1/cms:CMSE_1/caloBase:CALO_1",   4, kTRUE);
   make_node("/cms:World_1/cms:CMSE_1/muonBase:MUON_1",   4, kTRUE);

   gEve->FullRedraw3D(kTRUE);

   std_camera_clip();
}
