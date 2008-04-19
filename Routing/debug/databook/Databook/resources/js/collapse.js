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
// Collapse related functions
// NOTE: openImg and closeImg must be defined outside
//

function showBranch(branch){
    var objBranch = document.getElementById(branch).style;
    var obj = document.getElementById('I'+branch);
    
    if (objBranch.display=="none" || (objBranch.display=="" && document.getElementById(branch).getAttribute('hide')=="true")) {
    	objBranch.display="block";
        obj.innerHTML = '<img border="0" src="'+openImg+'"/>';
    } else {
    	objBranch.display="none";
        obj.innerHTML = '<img border="0" src="'+closeImg+'"/>';
    }
}

