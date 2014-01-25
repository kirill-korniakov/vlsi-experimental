#include "HDesign.h"
#include "TimingArcsBuilder.h"
#include "Utils.h"
#include "Auxiliary.h"
#include "si2dr_liberty.h"

struct LutTable
{
  si2drStringT name;

  si2drStringT variable_1;
  si2drStringT variable_2;
  si2drStringT variable_3;

  si2drStringT index_1;
  si2drStringT index_2;
  si2drStringT index_3;

  TemplateTypes<si2drStringT>::vector values;

  LutTable(): name(0),
      index_1(0), variable_1(0),
      index_2(0), variable_2(0),
      index_3(0), variable_3(0)
  {
  }
};

struct LIBParserData
{
  HDesign& Design;
  TemplateTypes<LutTable>::list LutTemplates;
  Builders::TimingArcsBuilder   ArcsBuilder;
  double time_to_ns_factor;
  double capacitance_to_pf_factor;
  double resistance_to_kohm_factor;

  LIBParserData(HDesign &hd) : Design(hd), ArcsBuilder(&hd)
  {
    time_to_ns_factor = 1.0;
    capacitance_to_pf_factor = 1.0;
    resistance_to_kohm_factor = 1.0;
  }

  LutTable* GetLutTemplateByName(si2drStringT name)
  {
    for (TemplateTypes<LutTable>::list::iterator i = LutTemplates.begin(); i != LutTemplates.end(); ++i)
      if (strcmp(i->name, name) == 0)
        return &(*i);
    return 0;
  }
};


void ParseNumbers(si2drStringT str, double* storage, int size)
{
  char* start_pos = str;
  int count_read = 0;
  while (*start_pos != 0 && count_read < size)
  {
    char *end_pos = start_pos;
    while (*end_pos != ',' && *end_pos != 0) end_pos++;
    char oldch = *end_pos;
    *end_pos = 0;
    storage[count_read++] = atof(start_pos);
    *end_pos = oldch;
    if (oldch == 0) return;
    start_pos = end_pos + 1;
  }
}

void ExtractPhisicsFromLUT(LutTable& tbl, double& a0, double& a1, double& a2)
{
  if (tbl.index_1 != 0 && tbl.index_2 != 0 /*&& tbl.index_3 == 0*/)
  {
    int idx1_size = 1;
    for (char* pos1 = tbl.index_1; *pos1 != 0; ++pos1)
      if (*pos1 == ',') idx1_size++;
    int idx2_size = 1;
    for (char* pos2 = tbl.index_2; *pos2 != 0; ++pos2)
      if (*pos2 == ',') idx2_size++;

    double *idx1 = new double[idx1_size];
    double *idx2 = new double[idx2_size];
    double *vals = new double[idx1_size * idx2_size];

    ParseNumbers(tbl.index_1, idx1, idx1_size);
    ParseNumbers(tbl.index_2, idx2, idx2_size);

    for (int i = 0; i < idx1_size; ++i)
      ParseNumbers(tbl.values[i], vals + i * idx2_size, idx2_size);
    
    Aux::MNK(idx1, idx2, vals, idx1_size, idx2_size, a0, a1, a2);

    delete[] idx1;
    delete[] idx2;
    delete[] vals;
  }
  else
  {
    LOGCRITICAL("Unsupported LUT. Unable to extract phisical parameters.");
  }
}

string UndecoratePinName(si2drStringT name)
{
  if(name == 0) return string();
  int i = 0;
  while(name[i] != 0 && !isalpha(name[i])) i++;
  int j = i;
  while(name[j] != 0 && isalnum(name[j])) j++;
  return string(name + i, name + j);
}

string UndecoratePinNameSavingInverse(si2drStringT name)
{
  if(name == 0) return string();
  int i = 0;
  while(name[i] != 0 && !isalpha(name[i]) && name[i] != '!') i++;
  int j = i;
  while(name[j] != 0 && (isalnum(name[j]) || name[j] == '!')) j++;
  return string(name + i, name + j);
}

si2drStringT GetOneName(si2drGroupIdT g2)
{
  si2drErrorT err;
  si2drNamesIdT names = si2drGroupGetNames(g2, &err);
  ASSERT(err == SI2DR_NO_ERROR);
  si2drStringT str = si2drIterNextName(names, &err);
  ASSERT(err == SI2DR_NO_ERROR);
  si2drIterQuit(names, &err);
  ASSERT(err == SI2DR_NO_ERROR);
  return str;
}

si2drStringT GetSimpleStringAttributeValue(si2drGroupIdT group, si2drStringT attr_name)
{
  si2drErrorT err;
  si2drAttrIdT attr = si2drGroupFindAttrByName(group, attr_name, &err);
  ASSERT(err == SI2DR_NO_ERROR);
  bool has_attr = !si2drObjectIsNull(attr, &err);
  ASSERT(err == SI2DR_NO_ERROR);
  if (has_attr)
  {
    si2drAttrTypeT at = si2drAttrGetAttrType(attr, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    ASSERT(at == SI2DR_SIMPLE);
    si2drValueTypeT vtype = si2drSimpleAttrGetValueType(attr, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    ASSERT(vtype == SI2DR_STRING);
    si2drStringT val = si2drSimpleAttrGetStringValue(attr, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    return val;
  }
  else
    return 0;
}

si2drStringT GetSingleStringFromAttr(si2drAttrIdT attr)
{
  si2drErrorT err;
  si2drAttrTypeT at = si2drAttrGetAttrType(attr, &err);
  ASSERT(err == SI2DR_NO_ERROR);

  if (at == SI2DR_SIMPLE)
  {
    si2drValueTypeT vtype = si2drSimpleAttrGetValueType(attr, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    ASSERT(vtype == SI2DR_STRING);
    si2drStringT val = si2drSimpleAttrGetStringValue(attr, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    return val;
  }
  else if (at == SI2DR_COMPLEX)
  {
    si2drValueTypeT vtype;
    si2drInt32T     intgr;
    si2drFloat64T   float64;
    si2drStringT    string;
    si2drBooleanT   boolval;
    si2drExprT      *exprp;
    si2drStringT    val;
    
    si2drValuesIdT vals = si2drComplexAttrGetValues(attr, &err);
    ASSERT(err == SI2DR_NO_ERROR);

    si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string, &boolval, &exprp, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    ASSERT(vtype != SI2DR_UNDEFINED_VALUETYPE);
    ASSERT(vtype == SI2DR_STRING);
    val = string;

    si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string, &boolval, &exprp, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    ASSERT(vtype == SI2DR_UNDEFINED_VALUETYPE);
    si2drIterQuit(vals, &err);
    ASSERT(err == SI2DR_NO_ERROR);

    return val;
  }
  return 0;
}

si2drStringT GetStringAttributeValue(si2drGroupIdT group, si2drStringT attr_name)
{
  si2drErrorT err;
  si2drAttrIdT attr = si2drGroupFindAttrByName(group, attr_name, &err);
  ASSERT(err == SI2DR_NO_ERROR || err == SI2DR_OBJECT_NOT_FOUND);
  bool has_attr =  !si2drObjectIsNull(attr, &err);
  ASSERT(err == SI2DR_NO_ERROR);
  return has_attr ? GetSingleStringFromAttr(attr) : 0;
}

si2drFloat64T GetSingleFloatFromAttr(si2drAttrIdT attr)
{
  si2drErrorT err;
  si2drAttrTypeT at = si2drAttrGetAttrType(attr, &err);
  ASSERT(err == SI2DR_NO_ERROR);

  if (at == SI2DR_SIMPLE)
  {
    si2drValueTypeT vtype = si2drSimpleAttrGetValueType(attr, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    ASSERT(vtype == SI2DR_FLOAT64 || vtype == SI2DR_INT32);

    si2drFloat64T val;
    if (vtype == SI2DR_FLOAT64)
      val = si2drSimpleAttrGetFloat64Value(attr, &err);
    else if (vtype == SI2DR_INT32)
      val = si2drSimpleAttrGetInt32Value(attr, &err);
    ASSERT(err == SI2DR_NO_ERROR);

    return val;
  }
  else if (at == SI2DR_COMPLEX)
  {
    si2drValueTypeT vtype;
    si2drInt32T     intgr;
    si2drFloat64T   float64;
    si2drStringT    string;
    si2drBooleanT   boolval;
    si2drExprT      *exprp;
    si2drFloat64T    val;
    
    si2drValuesIdT vals = si2drComplexAttrGetValues(attr, &err);
    ASSERT(err == SI2DR_NO_ERROR);

    si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string, &boolval, &exprp, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    ASSERT(vtype != SI2DR_UNDEFINED_VALUETYPE);
    ASSERT(vtype == SI2DR_FLOAT64);
    val = float64;

    si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string, &boolval, &exprp, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    ASSERT(vtype == SI2DR_UNDEFINED_VALUETYPE);
    si2drIterQuit(vals, &err);
    ASSERT(err == SI2DR_NO_ERROR);

    return val;
  }
  return 0;
}

si2drFloat64T GetFloatAttributeValue(si2drGroupIdT group, si2drStringT attr_name)
{
  si2drErrorT err;
  si2drAttrIdT attr = si2drGroupFindAttrByName(group, attr_name, &err);
  ASSERT(err == SI2DR_NO_ERROR || err == SI2DR_OBJECT_NOT_FOUND);
  bool has_attr =  !si2drObjectIsNull(attr, &err);
  ASSERT(err == SI2DR_NO_ERROR);
  return has_attr ? GetSingleFloatFromAttr(attr) : 0;
}

si2drBooleanT GetSingleBoolFromAttr(si2drAttrIdT attr)
{
  si2drErrorT err;
  si2drAttrTypeT at = si2drAttrGetAttrType(attr, &err);
  ASSERT(err == SI2DR_NO_ERROR);

  if (at == SI2DR_SIMPLE)
  {
    si2drValueTypeT vtype = si2drSimpleAttrGetValueType(attr, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    ASSERT(vtype == SI2DR_BOOLEAN);
    si2drBooleanT val = si2drSimpleAttrGetBooleanValue(attr, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    return val;
  }
  else if (at == SI2DR_COMPLEX)
  {
    si2drValueTypeT vtype;
    si2drInt32T     intgr;
    si2drFloat64T   float64;
    si2drStringT    string;
    si2drBooleanT   boolval;
    si2drExprT      *exprp;
    si2drBooleanT    val;
    
    si2drValuesIdT vals = si2drComplexAttrGetValues(attr, &err);
    ASSERT(err == SI2DR_NO_ERROR);

    si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string, &boolval, &exprp, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    ASSERT(vtype != SI2DR_UNDEFINED_VALUETYPE);
    ASSERT(vtype == SI2DR_BOOLEAN);
    val = boolval;

    si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string, &boolval, &exprp, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    ASSERT(vtype == SI2DR_UNDEFINED_VALUETYPE);
    si2drIterQuit(vals, &err);
    ASSERT(err == SI2DR_NO_ERROR);

    return val;
  }
  return si2drBooleanT(0);
}

si2drBooleanT GetBoolAttributeValue(si2drGroupIdT group, si2drStringT attr_name)
{
  si2drErrorT err;
  si2drAttrIdT attr = si2drGroupFindAttrByName(group, attr_name, &err);
  ASSERT(err == SI2DR_NO_ERROR || err == SI2DR_OBJECT_NOT_FOUND);
  bool has_attr =  !si2drObjectIsNull(attr, &err);
  ASSERT(err == SI2DR_NO_ERROR);
  return has_attr ? GetSingleBoolFromAttr(attr) : si2drBooleanT(0);
}

LutTable ReadLUT(si2drGroupIdT lut_group, LutTable* lut_template)
{
  LutTable res;

  res.name = GetOneName(lut_group);
  res.variable_1 = GetStringAttributeValue(lut_group, "variable_1");
  res.variable_2 = GetStringAttributeValue(lut_group, "variable_2");
  res.variable_3 = GetStringAttributeValue(lut_group, "variable_3");
  res.index_1 = GetStringAttributeValue(lut_group, "index_1");
  res.index_2 = GetStringAttributeValue(lut_group, "index_2");
  res.index_3 = GetStringAttributeValue(lut_group, "index_3");
 
  if(lut_template != 0)
  {
    ASSERT(lut_template->name && strcmp(lut_template->name, res.name) == 0);
    
    if(res.index_1 == 0) res.index_1 = lut_template->index_1;
    if(res.index_2 == 0) res.index_2 = lut_template->index_2;
    if(res.index_3 == 0) res.index_3 = lut_template->index_3;
    if(res.variable_1 == 0) res.variable_1 = lut_template->variable_1;
    if(res.variable_2 == 0) res.variable_2 = lut_template->variable_2;
    if(res.variable_3 == 0) res.variable_3 = lut_template->variable_3;

    {// read values
      si2drErrorT err;
      si2drAttrIdT attr_values = si2drGroupFindAttrByName(lut_group, "values", &err);
      ASSERT(err == SI2DR_NO_ERROR);
      bool has_values = !si2drObjectIsNull(attr_values, &err);
      ASSERT(err == SI2DR_NO_ERROR);
      if (has_values)
      {
        si2drAttrTypeT at = si2drAttrGetAttrType(attr_values, &err);
        ASSERT(err == SI2DR_NO_ERROR);
        ASSERT(at == SI2DR_COMPLEX);
        si2drValuesIdT vals = si2drComplexAttrGetValues(attr_values, &err);
        ASSERT(err == SI2DR_NO_ERROR);
        si2drValueTypeT vtype;
        si2drInt32T     intgr;
        si2drFloat64T   float64;
        si2drStringT    string;
        si2drBooleanT   boolval;
        si2drExprT      *exprp;

        si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string, &boolval, &exprp, &err);
        ASSERT(err == SI2DR_NO_ERROR);
        ASSERT(vtype == SI2DR_STRING);

        while (vtype != SI2DR_UNDEFINED_VALUETYPE)
        {
          res.values.push_back(string);
          si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string, &boolval, &exprp, &err);
          ASSERT(err == SI2DR_NO_ERROR);
          ASSERT(vtype == SI2DR_STRING || vtype == SI2DR_UNDEFINED_VALUETYPE);
        }

        si2drIterQuit(vals, &err);
        ASSERT(err == SI2DR_NO_ERROR);
      }
    }
  }
  
  return res;
}

void ProcessSeqElementGroup(si2drGroupIdT seq_group, LIBParserData& data)
{
  {//clear 
    si2drStringT clear = GetStringAttributeValue(seq_group, "clear");
    if (clear)
    {
      HPinType pin = Utils::FindMacroTypePinByName(data.Design, data.ArcsBuilder.MacroType(), UndecoratePinName(clear));
      if(!::IsNull(pin))
        data.Design.Set<HPinType::Function>(pin, PinFunction_Clear);
    }
  }

  {//preset 
    si2drStringT preset = GetStringAttributeValue(seq_group, "preset");
    if (preset)
    {
      HPinType pin = Utils::FindMacroTypePinByName(data.Design, data.ArcsBuilder.MacroType(), UndecoratePinName(preset));
      if(!::IsNull(pin))
        data.Design.Set<HPinType::Function>(pin, PinFunction_Preset);
    }
  }
}

void ProcessFFGroup(si2drGroupIdT ff_group, LIBParserData& data)
{
  ProcessSeqElementGroup(ff_group, data);
  
  //process clock pin information
  si2drStringT clocked_on = GetStringAttributeValue(ff_group, "clocked_on");
  if (clocked_on)
  {
    string pin_name = UndecoratePinNameSavingInverse(clocked_on);
    if(pin_name.size() > 0 && pin_name[0] == '!')
      data.Design.Set<HMacroType::Type>(data.ArcsBuilder.MacroType(), MacroType_FEFF);
    else
      data.Design.Set<HMacroType::Type>(data.ArcsBuilder.MacroType(), MacroType_REFF);

    HPinType pin = Utils::FindMacroTypePinByName(data.Design, data.ArcsBuilder.MacroType(), UndecoratePinName(clocked_on));
    if(!::IsNull(pin))
      data.Design.Set<HPinType::Function>(pin, PinFunction_Clock);
  }
  else
    data.Design.Set<HMacroType::Type>(data.ArcsBuilder.MacroType(), MacroType_REFF);
}

void ProcessLatchGroup(si2drGroupIdT latch_group, LIBParserData& data)
{
  ProcessSeqElementGroup(latch_group, data);
  
  //process enable pin (=clock) information
  si2drStringT enable = GetStringAttributeValue(latch_group, "enable");
  if (enable)
  {
    string pin_name = UndecoratePinNameSavingInverse(enable);
    if(pin_name.size() > 0 && pin_name[0] == '!')
      data.Design.Set<HMacroType::Type>(data.ArcsBuilder.MacroType(), MacroType_LLSL);
    else
      data.Design.Set<HMacroType::Type>(data.ArcsBuilder.MacroType(), MacroType_HLSL);

    HPinType pin = Utils::FindMacroTypePinByName(data.Design, data.ArcsBuilder.MacroType(), UndecoratePinName(enable));
    if(!::IsNull(pin))
      data.Design.Set<HPinType::Function>(pin, PinFunction_Clock);
  }
  else
    data.Design.Set<HMacroType::Type>(data.ArcsBuilder.MacroType(), MacroType_HLSL);
}

void ReadTimingLUTs(si2drGroupIdT timing_group, LIBParserData& data)
{
  si2drErrorT err;
  si2drGroupsIdT groups = si2drGroupGetGroups(timing_group, &err);
  ASSERT(err == SI2DR_NO_ERROR);    

  si2drGroupIdT group = si2drIterNextGroup(groups, &err);
  ASSERT(err == SI2DR_NO_ERROR);

  while (!si2drObjectIsNull(group, &err))
  {
    ASSERT(err == SI2DR_NO_ERROR);

    si2drStringT template_name = GetOneName(group);
    si2drStringT group_type = si2drGroupGetGroupType(group, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    
    if (strcmp(group_type, "cell_rise") == 0)
    {
      LutTable tbl = ReadLUT(group, data.GetLutTemplateByName(template_name));
      
      ASSERT(tbl.variable_1
        && tbl.variable_2
        && (strcmp(tbl.variable_1, "input_net_transition") == 0
          && strcmp(tbl.variable_2, "total_output_net_capacitance") == 0
          || strcmp(tbl.variable_1, "total_output_net_capacitance") == 0
          && strcmp(tbl.variable_2, "input_net_transition") == 0)
        );
      
      double t,r,a;
      ExtractPhisicsFromLUT(tbl, t, r, a);
      
      if (strcmp(tbl.variable_1, "input_net_transition") == 0)
        ::Swap(&r, &a);
      r = std::max(r, 0.0);
      t = std::max(t, 0.0);

      data.ArcsBuilder->SetResistanceRise(r * data.resistance_to_kohm_factor);
      data.ArcsBuilder->SetTIntrinsicRise(t * data.time_to_ns_factor);

      //printf("%12.9f %12.9f %12.9f\n", t,r,a);
    }
    else if (strcmp(group_type, "cell_fall") == 0)
    {
      LutTable tbl = ReadLUT(group, data.GetLutTemplateByName(template_name));

      ASSERT(tbl.variable_1
        && tbl.variable_2
        && (strcmp(tbl.variable_1, "input_net_transition") == 0
          && strcmp(tbl.variable_2, "total_output_net_capacitance") == 0
          || strcmp(tbl.variable_1, "total_output_net_capacitance") == 0
          && strcmp(tbl.variable_2, "input_net_transition") == 0)
        );
      
      double t,r,a;
      ExtractPhisicsFromLUT(tbl, t, r, a);
      
      if (strcmp(tbl.variable_1, "input_net_transition") == 0)
        ::Swap(&r, &a);
      r = std::max(r, 0.0);
      t = std::max(t, 0.0);

      data.ArcsBuilder->SetResistanceFall(r * data.resistance_to_kohm_factor);
      data.ArcsBuilder->SetTIntrinsicFall(t * data.time_to_ns_factor);
      
      //printf("%12.9f %12.9f %12.9f\n", t,r,a);
    }    

    //fetch next item
    group = si2drIterNextGroup(groups, &err);
    ASSERT(err == SI2DR_NO_ERROR);
  }
  si2drIterQuit(groups, &err);
  ASSERT(err == SI2DR_NO_ERROR);
}

void ReadConstraintLUTs(si2drGroupIdT timing_group, LIBParserData& data)
{
  si2drErrorT err;
  si2drGroupsIdT groups = si2drGroupGetGroups(timing_group, &err);
  ASSERT(err == SI2DR_NO_ERROR);    

  si2drGroupIdT group = si2drIterNextGroup(groups, &err);
  ASSERT(err == SI2DR_NO_ERROR);

  while (!si2drObjectIsNull(group, &err))
  {
    ASSERT(err == SI2DR_NO_ERROR);

    si2drStringT template_name = GetOneName(group);
    si2drStringT group_type = si2drGroupGetGroupType(group, &err);
    ASSERT(err == SI2DR_NO_ERROR);
    
    if (strcmp(group_type, "rise_constraint") == 0)
    {
      LutTable tbl = ReadLUT(group, data.GetLutTemplateByName(template_name));

      ASSERT(tbl.variable_1 && tbl.variable_2
          && (strcmp(tbl.variable_1, "constrained_pin_transition") == 0
          && strcmp(tbl.variable_2, "related_pin_transition") == 0
          || strcmp(tbl.variable_1, "related_pin_transition") == 0
          && strcmp(tbl.variable_2, "constrained_pin_transition") == 0)
        );
      
      double t,x1,x2;
      ExtractPhisicsFromLUT(tbl, t, x1, x2);
      
      t = std::max(t, 0.0);
      
      data.ArcsBuilder->SetResistanceRise(0.0);
      data.ArcsBuilder->SetTIntrinsicRise(t * data.time_to_ns_factor);

      //printf("%12.9f %12.9f %12.9f **\n", t,x1,x2);
    }
    else if (strcmp(group_type, "fall_constraint") == 0)
    {
      LutTable tbl = ReadLUT(group, data.GetLutTemplateByName(template_name));
      
      ASSERT(tbl.variable_1
        && tbl.variable_2
        && (strcmp(tbl.variable_1, "constrained_pin_transition") == 0
          && strcmp(tbl.variable_2, "related_pin_transition") == 0
          || strcmp(tbl.variable_1, "related_pin_transition") == 0
          && strcmp(tbl.variable_2, "constrained_pin_transition") == 0)
        );

      double t,x1,x2;
      ExtractPhisicsFromLUT(tbl, t, x1, x2);
      
      t = std::max(t, 0.0);
      
      data.ArcsBuilder->SetResistanceFall(0.0);
      data.ArcsBuilder->SetTIntrinsicFall(t * data.time_to_ns_factor);
      
      //printf("%12.9f %12.9f %12.9f **\n", t,x1,x2);
    }    

    //fetch next item
    group = si2drIterNextGroup(groups, &err);
    ASSERT(err == SI2DR_NO_ERROR);
  }
  si2drIterQuit(groups, &err);
  ASSERT(err == SI2DR_NO_ERROR);
}

void ProcessTimingGroup(si2drGroupIdT timing_group, LIBParserData& data)
{
  si2drStringT related_pin = GetStringAttributeValue(timing_group, "related_pin");
  si2drStringT when = GetStringAttributeValue(timing_group, "when");
  si2drStringT timing_type = GetStringAttributeValue(timing_group, "timing_type");
  si2drStringT timing_sense = GetStringAttributeValue(timing_group, "timing_sense");

  TimingSense tsense = Utils::GetTimingSense(timing_sense);
  TimingType  ttype  = Utils::GetTimingType (timing_type );

  switch (ttype)
  {
  case TimingType_Combinational:
  case TimingType_FallingEdge:
  case TimingType_RisingEdge:
    data.ArcsBuilder.AddNewArc(related_pin);
    data.ArcsBuilder->SetTimingSense(tsense);
    data.ArcsBuilder->SetTimingType(ttype);

    ReadTimingLUTs(timing_group, data);

    data.ArcsBuilder.ArcFinished();
    break;
  case TimingType_SetupRising:
  case TimingType_SetupFalling:
    data.ArcsBuilder.AddNewArc(related_pin);
    data.ArcsBuilder->SetTimingSense(tsense);
    data.ArcsBuilder->SetTimingType(ttype);

    ReadConstraintLUTs(timing_group, data);

    data.ArcsBuilder.ArcFinished();
    break;

  case TimingType_HoldRising:
  case TimingType_HoldFalling:
  case TimingType_Preset:
  case TimingType_Clear:
  case TimingType_RecoveryRising:
  case TimingType_RecoveryFalling:
  case TimingType_RemovalRising:
  case TimingType_RemovalFalling:
  case TimingType_3StateEnable:
  case TimingType_3StateDisable:
  case TimingType_NochangeHigh:
  case TimingType_NochangeLow:
    //ignore these arcs
    break;
  default:
    ASSERT("Unknown arc type detected.");
    break;    
  }
}

void ProcessPin(si2drGroupIdT pin_group, LIBParserData& data)
{
  si2drStringT pin_direction = GetStringAttributeValue(pin_group, "direction");
  if (strcmp(pin_direction, "internal") == 0)
    return;

  si2drStringT pin_name = GetOneName(pin_group);
  data.ArcsBuilder.OpenPinType(pin_name);

  //override pin direction
  if (strcmp(pin_direction, "input") == 0)
    data.Design.Set<HPinType::Direction>(data.ArcsBuilder.PinType(), PinDirection_INPUT);
  else if (strcmp(pin_direction, "output") == 0)
    data.Design.Set<HPinType::Direction>(data.ArcsBuilder.PinType(), PinDirection_OUTPUT);
  else if (strcmp(pin_direction, "inout") == 0)
    data.Design.Set<HPinType::Direction>(data.ArcsBuilder.PinType(), PinDirection_INOUT);

  //mark pin as clock if needed
  si2drBooleanT isClockPin = GetBoolAttributeValue(pin_group, "clock");
  if (isClockPin == SI2DR_TRUE)
    data.Design.Set<HPinType::Function>(data.ArcsBuilder.PinType(), PinFunction_Clock);

  //mark another pin as 3state if needed
  string threeStatePinName = UndecoratePinName(GetStringAttributeValue(pin_group, "three_state"));
  if (!threeStatePinName.empty())
  {
    HPinType pin = Utils::FindMacroTypePinByName(data.Design, data.ArcsBuilder.MacroType(), threeStatePinName);
    if(!::IsNull(pin))
      data.Design.Set<HPinType::Function>(pin, PinFunction_3State);
  }

  //set pin capacitances
  si2drFloat64T pin_capacitance = GetFloatAttributeValue(pin_group, "capacitance");
  si2drFloat64T pin_fall_capacitance = GetFloatAttributeValue(pin_group, "fall_capacitance");
  si2drFloat64T pin_rise_capacitance = GetFloatAttributeValue(pin_group, "rise_capacitance");
  data.Design.Set<HPinType::Capacitance>(data.ArcsBuilder.PinType(), pin_capacitance * data.capacitance_to_pf_factor);
  data.Design.Set<HPinType::FallCapacitance>(data.ArcsBuilder.PinType(), pin_fall_capacitance * data.capacitance_to_pf_factor);
  data.Design.Set<HPinType::RiseCapacitance>(data.ArcsBuilder.PinType(), pin_rise_capacitance * data.capacitance_to_pf_factor);

  si2drErrorT err;
  si2drGroupsIdT groups = si2drGroupGetGroups(pin_group, &err);
  ASSERT(err == SI2DR_NO_ERROR);

  si2drGroupIdT group = si2drIterNextGroup(groups, &err);
  ASSERT(err == SI2DR_NO_ERROR);

  while (!si2drObjectIsNull(group, &err))
  {
    ASSERT(err == SI2DR_NO_ERROR);

    si2drStringT group_type = si2drGroupGetGroupType(group, &err);
    ASSERT(err == SI2DR_NO_ERROR);

    if (strcmp(group_type, "timing") == 0)
      ProcessTimingGroup(group, data);

    //fetch next item
    group = si2drIterNextGroup(groups, &err);
    ASSERT(err == SI2DR_NO_ERROR);
  }

  si2drIterQuit(groups, &err);
  ASSERT(err == SI2DR_NO_ERROR);

  data.ArcsBuilder.ClosePinType();
}

void ProcessCell(si2drGroupIdT cell_group, LIBParserData& data)
{
  si2drErrorT err;
  si2drStringT cell_name = GetOneName(cell_group);
  data.ArcsBuilder.OpenMacroType(cell_name);

  si2drGroupsIdT groups = si2drGroupGetGroups(cell_group, &err);
  ASSERT(err == SI2DR_NO_ERROR);

  si2drGroupIdT group = si2drIterNextGroup(groups, &err);
  ASSERT(err == SI2DR_NO_ERROR);

  while (!si2drObjectIsNull(group, &err))
  {
    ASSERT(err == SI2DR_NO_ERROR);

    si2drStringT group_type = si2drGroupGetGroupType(group, &err);
    ASSERT(err == SI2DR_NO_ERROR);

    if (strcmp(group_type, "pin") == 0)
      ProcessPin(group, data);
    else if (strcmp(group_type, "ff") == 0)
      ProcessFFGroup(group, data);
    else if (strcmp(group_type, "latch") == 0)
      ProcessLatchGroup(group, data);

    //fetch next item
    group = si2drIterNextGroup(groups, &err);
    ASSERT(err == SI2DR_NO_ERROR);
  }

  si2drIterQuit(groups, &err);
  ASSERT(err == SI2DR_NO_ERROR);

  data.ArcsBuilder.CloseMacroType();
}


void ProcessLibraryTopGroups(si2drGroupsIdT groups, LIBParserData& data)
{
  si2drErrorT err;

  si2drGroupIdT group = si2drIterNextGroup(groups, &err);
  ASSERT(err == SI2DR_NO_ERROR);

  while (!si2drObjectIsNull(group, &err))
  {
    ASSERT(err == SI2DR_NO_ERROR);

    si2drStringT group_type = si2drGroupGetGroupType(group, &err);
    ASSERT(err == SI2DR_NO_ERROR);

    if (strcmp(group_type, "lu_table_template") == 0)
      data.LutTemplates.push_back(ReadLUT(group, 0));
    else if (strcmp(group_type, "cell") == 0)
      ProcessCell(group, data);

    //fetch next item
    group = si2drIterNextGroup(groups, &err);
    ASSERT(err == SI2DR_NO_ERROR);
  }
}

void ReadLibraries(si2drGroupsIdT groups, LIBParserData& data)
{
  si2drErrorT err;
  
  si2drGroupIdT group = si2drIterNextGroup(groups, &err);
  ASSERT(err == SI2DR_NO_ERROR);

  int libs_count = 0;
  while (!si2drObjectIsNull(group, &err))
  {
    ASSERT(err == SI2DR_NO_ERROR);
    
    //check that current library is single available
    libs_count++;
    if(libs_count > 1)
    {
      LOGWARNING("Currently we support only one library in .lib file.\nAll subsequent libraries will be ignored.");
      break;
    }

    //print library name
    ALERT("Extracting data from library %s", GetOneName(group));

    //ensure that "delay_model : table_lookup;"
    if (strcmp(GetSimpleStringAttributeValue(group, "delay_model"), "table_lookup") != 0)
    {
      LOGCRITICAL("Only \"table_lookup\" model currently supported.");
    }

    //ensure "time_unit : "1ns";"
    CRITICAL_ASSERT(strcmp(GetSimpleStringAttributeValue(group, "time_unit"), "1ns") == 0);
    data.time_to_ns_factor = 1.0; //time convert factor is set to 1.0

    //ensure "pulling_resistance_unit : "1kohm";"
    CRITICAL_ASSERT(strcmp(GetSimpleStringAttributeValue(group, "pulling_resistance_unit"), "1kohm") == 0);
    data.resistance_to_kohm_factor = 1.0; //resistance convert factor is set to 1.0 (we use kohms)

    {//ensure "capacitive_load_unit (UNITS,pf);"
      si2drAttrIdT attr_cload_unit = si2drGroupFindAttrByName(group, "capacitive_load_unit", &err);
      ASSERT(err == SI2DR_NO_ERROR);
      bool has_cload_unit = !si2drObjectIsNull(attr_cload_unit, &err);
      ASSERT(err == SI2DR_NO_ERROR);
      if (has_cload_unit)
      {
        si2drAttrTypeT at_cload_unit = si2drAttrGetAttrType(attr_cload_unit, &err);
        ASSERT(err == SI2DR_NO_ERROR);
        ASSERT(at_cload_unit == SI2DR_COMPLEX);
        si2drValuesIdT vals = si2drComplexAttrGetValues(attr_cload_unit, &err);
        ASSERT(err == SI2DR_NO_ERROR);
        si2drValueTypeT vtype;
        si2drInt32T     intgr;
        si2drFloat64T   float64;
        si2drStringT    string;
        si2drBooleanT   boolval;
        si2drExprT      *exprp;
        si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string, &boolval, &exprp, &err);
        ASSERT(err == SI2DR_NO_ERROR);
        ASSERT(vtype == SI2DR_INT32);
        
        data.capacitance_to_pf_factor = intgr;

        si2drIterNextComplexValue(vals, &vtype, &intgr, &float64, &string, &boolval, &exprp, &err);
        ASSERT(err == SI2DR_NO_ERROR);
        ASSERT(vtype != SI2DR_UNDEFINED_VALUETYPE);
        ASSERT(vtype == SI2DR_STRING);
        CRITICAL_ASSERT(strcmp(string, "pf") == 0);
        si2drIterQuit(vals, &err);
        ASSERT(err == SI2DR_NO_ERROR);
      }
    }

    //process subitems (table templates and cells)
    si2drGroupsIdT gs2 = si2drGroupGetGroups(group, &err);
    ASSERT(err == SI2DR_NO_ERROR);

    ProcessLibraryTopGroups(gs2, data);

    si2drIterQuit(gs2, &err);
    ASSERT(err == SI2DR_NO_ERROR);

    //fetch next library
    group = si2drIterNextGroup(groups, &err);
  }
}

void ParseLIB(HDesign& design)
{
  ConfigContext ctx(design.cfg.OpenContext("LIBParser"));

  si2drErrorT err;
  ALERT("Lib file %s parsing started...", (const char*)design.cfg.ValueOf("benchmark.lib"));
  si2drPIInit(&err);
  si2drReadLibertyFile((char*)(const char*)design.cfg.ValueOf("benchmark.lib"), &err);
  if(err == SI2DR_INVALID_NAME )
  {
    GLOGCRITICAL(LOGINPLACE, "COULD NOT OPEN %s for parsing", (const char*)design.cfg.ValueOf("benchmark.lib"));
  }
  else if (err == SI2DR_SYNTAX_ERROR )
  {
    LOGCRITICAL("Syntax Errors were detected in the input file.");
  }
  si2drGroupsIdT groups = si2drPIGetGroups(&err);

  LIBParserData data(design);
  data.ArcsBuilder.TimingArcsStart(design.cfg.ValueOf(".startArcsCount", design.PinTypes.Count()));
  ReadLibraries(groups, data);
  data.ArcsBuilder.TimingArcsFinished();

  si2drIterQuit(groups, &err);
  si2drPIQuit(&err);
  ALERT("Lib parsing finished.");
}

//void PrintOneName(si2drGroupIdT g2)
//{
//  si2drErrorT err;
//  si2drNamesIdT names = si2drGroupGetNames(g2, &err);
//  si2drStringT str = si2drIterNextName(names,&err);
//  printf("%s", str);
//  si2drIterQuit(names,&err);
//  printf("\n");
//}
//
//void DumpDefines(si2drGroupIdT g2)
//{
//  si2drErrorT err;
//  si2drDefineIdT def;
//  si2drDefinesIdT defs = si2drGroupGetDefines(g2, &err);
//  for(def = si2drIterNextDefine(defs, &err);
//    !si2drObjectIsNull(def,&err);
//    def = si2drIterNextDefine(defs,&err))
//  {
//    si2drStringT nam, gnam;
//    si2drValueTypeT valtype;
//    si2drDefineGetInfo(def,&nam, &gnam, &valtype, &err);
//    printf("(!)DIFINE %s | %s\n", nam, gnam);
//
//  }
//  si2drIterQuit(defs,&err);
//}
//
//void DumpAttrs(si2drGroupIdT g2)
//{
//  si2drErrorT err;
//  si2drAttrIdT  attr;
//  si2drAttrsIdT attrs = si2drGroupGetAttrs(g2, &err);
//  for(attr = si2drIterNextAttr(attrs,&err);
//    !si2drObjectIsNull(attr,&err);
//    attr = si2drIterNextAttr(attrs,&err))
//  {
//    si2drAttrTypeT at = si2drAttrGetAttrType(attr,&err);
//    si2drValuesIdT vals;
//    si2drValueTypeT vtype;
//    si2drInt32T     intgr;
//    si2drFloat64T   float64;
//    si2drStringT    string, nam= si2drAttrGetName(attr,&err);
//    si2drBooleanT   boolval;
//    si2drBooleanT   is_var;
//    si2drExprT      *exprp;
//    int first;
//
//    if( at == SI2DR_SIMPLE && 
//      (!strcmp(nam,"default_operating_conditions")
//      || !strcmp(nam,"default_wire_load")
//      || !strcmp(nam,"default_wire_load_selection")))
//      continue;
//
//    printf("%s", nam);
//
//    switch( at )
//    {
//    case SI2DR_SIMPLE:
//      vtype = si2drSimpleAttrGetValueType(attr,&err);
//      is_var = si2drSimpleAttrGetIsVar(attr,&err);
//      switch( vtype )
//      {
//      case SI2DR_STRING:
//        if( is_var )
//          printf(" = \"%s\";\n", si2drSimpleAttrGetStringValue(attr,&err));
//        else
//          printf(" : \"%s\";\n", si2drSimpleAttrGetStringValue(attr,&err));
//        break;
//
//      case SI2DR_FLOAT64:
//        if( is_var )
//          printf(" = %1.6f;\n", si2drSimpleAttrGetFloat64Value(attr,&err));
//        else if (    !strcmp(nam, "key_bit")
//          || !strcmp(nam, "address_width") 
//          || !strcmp(nam, "word_width") 
//          || !strcmp(nam, "key_seed") 
//          || !strcmp(nam, "tracks") 
//          || !strcmp(nam, "divided_by") 
//          || !strcmp(nam, "multiplied_by") 
//          || !strcmp(nam, "bit_from") 
//          || !strcmp(nam, "bit_to") 
//          || !strcmp(nam, "bit_width") )
//          printf(" : %d;\n", (int)si2drSimpleAttrGetFloat64Value(attr,&err));
//        else if(!strcmp(si2drGroupGetGroupType(g2,&err), "leakage_power") && !strcmp(nam, "value"))
//          printf(" : %1.6e;\n", si2drSimpleAttrGetFloat64Value(attr,&err));
//        else if (!strcmp(nam, "cell_leakage_power"))
//          printf(" : %1.6e;\n", si2drSimpleAttrGetFloat64Value(attr,&err));
//        else if(!strcmp(si2drGroupGetGroupType(g2,&err), "gate_leakage")) 
//          printf(" : %1.6e;\n", si2drSimpleAttrGetFloat64Value(attr, &err));
//        else if(!strcmp(si2drGroupGetGroupType(g2,&err), "pg_current")) 
//          printf(" : %1.6e;\n", si2drSimpleAttrGetFloat64Value(attr, &err));
//        else if(!strcmp(si2drGroupGetGroupType(g2,&err), "intrinsic_capacitance")) 
//          printf(" : %1.6e;\n", si2drSimpleAttrGetFloat64Value(attr, &err));
//        else if(!strcmp(si2drGroupGetGroupType(g2,&err), "wire_load") && !strcmp(nam, "resistance"))
//          printf(" : %e;\n", si2drSimpleAttrGetFloat64Value(attr, &err));
//        else
//          printf(" : %1.6f;\n", si2drSimpleAttrGetFloat64Value(attr,&err));
//        break;
//
//      case SI2DR_INT32:
//        if( is_var )
//          printf(" = %d;\n", (int)si2drSimpleAttrGetInt32Value(attr,&err));
//        else
//          printf(" : %d;\n", (int)si2drSimpleAttrGetInt32Value(attr,&err));
//        break;
//
//      case SI2DR_EXPR:
//        {
//          //char *es = expr_string( si2drSimpleAttrGetExprValue(attr,&err));
//          si2drExprT *ttt = si2drSimpleAttrGetExprValue(attr,&err);
//          //if( is_var )
//          //printf(" = %s;\n", es);
//          //else
//          //printf(" : %s;\n", es);
//          /* expr strings in strtable OLD: free(es); */
//        }
//        break;
//
//      case SI2DR_BOOLEAN:
//        if( is_var )
//        {
//          if( si2drSimpleAttrGetBooleanValue(attr,&err) )
//            printf(" = true;\n");
//          else
//            printf(" = false;\n");
//        }
//        else
//        {
//          if( si2drSimpleAttrGetBooleanValue(attr,&err) )
//            printf(" : true;\n");
//          else
//            printf(" : false;\n");
//        }
//        break;
//      default:
//        break;
//      }
//      break;
//
//    case SI2DR_COMPLEX:
//      vals = si2drComplexAttrGetValues(attr,&err);
//      first = 1;
//      printf("(");
//
//      for(si2drIterNextComplexValue(vals,&vtype, &intgr, &float64, &string, &boolval, &exprp, &err);
//        vtype != SI2DR_UNDEFINED_VALUETYPE;
//        si2drIterNextComplexValue(vals,&vtype, &intgr, &float64, &string, &boolval, &exprp, &err))
//      {
//        if( !first )
//          printf(",");
//        switch( vtype )
//        {
//        case SI2DR_STRING:
//          /* [20060809:nanda] Workaround for */
//          /* define_group attribute */
//          /* fprintf(of,"\"%s\"", string); */
//          /* if( !first )                  */
//          /* fprintf(of," \\\n%s	", indent2); */
//          if (!strcmp(nam, "define_group")) {
//            printf("%s", string);
//          } else {
//            if( !first )
//              printf(" \\\n%s	", "\t");
//            printf("\"%s\"", string);
//          }
//          break;
//
//        case SI2DR_FLOAT64:
//          if(!strcmp(nam, "curve_y") || !strcmp(nam, "vector"))
//            printf("%d", (int)float64);
//          else if(!strcmp(nam, "va_values") || !strcmp(nam, "nominal_va_values"))
//            printf("%1.6e", float64);
//          else
//            printf("%1.6f", float64);
//          break;
//
//        case SI2DR_INT32:
//          printf("%d", (int)intgr);
//          break;
//
//        case SI2DR_BOOLEAN:
//          printf("%d", boolval);
//          break;
//        default:
//          break;
//        }
//        first = 0;
//      }
//      si2drIterQuit(vals,&err);
//      printf(");\n");
//      break;
//    }
//  }
//  si2drIterQuit(attrs,&err);
//}
//
//void PrintNames(si2drGroupIdT g2)
//{
//  si2drErrorT err;
//  si2drStringT str;
//  int first = 1;
//  si2drNamesIdT names = si2drGroupGetNames(g2,&err);
//  for(str = si2drIterNextName(names,&err);
//    str && str[0];
//    str = si2drIterNextName(names,&err))
//  {
//    if( !first )
//      printf(",");
//    printf("%s", str);
//    first = 0;
//
//  }
//  si2drIterQuit(names,&err);
//  printf("\n");
//}
