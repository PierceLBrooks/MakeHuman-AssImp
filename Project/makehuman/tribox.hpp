//
//  tribox.hpp
//  VoxelHeatDiffuse
//
//  Created by MINGFENWANG on 2017/10/7.
//  Copyright © 2017年 MINGFENWANG. All rights reserved.
//

#ifndef tribox_hpp
#define tribox_hpp

namespace mh {
int planeBoxOverlap(float normal[3], float vert[3], float maxbox[3]);
int triBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3]);
}

#endif /* tribox_hpp */
