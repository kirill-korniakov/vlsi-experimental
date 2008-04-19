//
//     Copyright (C) 2006, Nangate Inc.
//     All rights reserved.                           
//
//	Copyright Notice:
//	
//		This document contains confidential and proprietary information.		
//		Reproduction or usage of this document, in part or whole, by any means,
//		electrical, mechanical, optical, chemical or otherwise is prohibited,
//		without written permission from Nangate Inc.
//		
//		The information contained herein is protected by Danish and international  
//		copyright laws.                                                            /
//	
//	Author: 
//		Joao Daniel Togni
//
//  Version: 
//		1.0
// 

//
// Tooltip related functions
//

nangate_tt = null;
nangate_image = null;

function showTT(id) {
  hideTT();
  var images = document.getElementById(id.substring(0, id.length - 3)).getElementsByTagName('div')[0].getElementsByTagName('img');
  if (images.length) {
    nangate_image = images[0];
    nangate_image.style.display = "inline";
  }
  nangate_tt = document.getElementById(id);
  nangate_tt.style.display = "block";
}

function hideTT() {
  if (nangate_tt)
    nangate_tt.style.display = "none";
  if (nangate_image)
    nangate_image.style.display = "none";
}
