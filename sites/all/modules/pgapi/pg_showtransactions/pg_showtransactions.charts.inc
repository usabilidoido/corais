<?php

function pg_showtransactions_overview() {

  $output .= pg_showtransactions_yearchart();
  $output .= pg_showtransactions_monthchart();

  return $output;
}



function pg_showtransactions_overviewsettings() {
  $charts=array(
     'line' => 'Line',
     'line_dot' => 'Line Dot',
     'line_hollow' => 'Line Hollow',
     'area_hollow' => 'Area Hollow',
     'bar' => 'Bar',
     'bar_filled' => 'Bar Filled',
     'bar_3D' => 'Bar 3D',
     'bar_glass' => 'Bar Glass',
     'bar_fade' => 'Bar Fade',
//     'pie' => 'Pie',
  );
  $form['overview_year'] = array (
    '#type'          => 'fieldset',
    '#title'         => 'Year Overview settings',
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  $form['overview_year']['pg_showtransactions_year_count'] = array (
    '#type'          => 'textfield',
    '#title'         => t('Years'),
    '#description'   => t('How much years would be show'),
    '#default_value' => variable_get('pg_showtransactions_year_count',4),
  );
  $form['overview_year']['pg_showtransactions_year_chart']= array (
    '#type'          => 'select',
    '#title'         => 'Year Chart type',
    '#description'   => t('Please select the chart for year'),
    '#default_value' => variable_get('pg_showtransactions_year_chart','bar_glass'),
    '#options'       => $charts,
  );
  $form['overview_year']['pg_showtransactions_year_height'] = array (
    '#type'          => 'textfield',
    '#title'         => t('Height'),
    '#description'   => t('Please enter height of chart'),
    '#default_value' => variable_get('pg_showtransactions_year_height',300),
  );
  $form['overview_year']['pg_showtransactions_year_width'] = array (
    '#type'          => 'textfield',
    '#title'         => t('Width'),
    '#description'   => t('Please enter width of chart'),
    '#default_value' => variable_get('pg_showtransactions_year_width',500),
  );
  $form['overview_year']['pg_showtransactions_year_bgcolor'] = array (
    '#type'          => 'textfield',
    '#title'         => t('Background color'),
    '#description'   => t('Please enter code of background color'),
    '#default_value' => variable_get('pg_showtransactions_year_bgcolor','#FFFFFF'),
  );

  $form['overview_month'] = array (
    '#type'          => 'fieldset',
    '#title'         => 'Month Overview settings',
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  $form['overview_month']['pg_showtransactions_month_count'] = array (
    '#type'          => 'textfield',
    '#title'         => t('Months'),
    '#description'   => t('How much month would be show'),
    '#default_value' => variable_get('pg_showtransactions_month_count',4),
  );
  $form['overview_month']['pg_showtransactions_month_chart']= array (
    '#type'          => 'select',
    '#title'         => 'Month Chart type',
    '#description'   => t('Please select the chart for month'),
    '#default_value' => variable_get('pg_showtransactions_month_chart','bar_glass'),
    '#options'       => $charts,
  );
  $form['overview_month']['pg_showtransactions_month_height'] = array (
    '#type'          => 'textfield',
    '#title'         => t('Height'),
    '#description'   => t('Please enter height of chart'),
    '#default_value' => variable_get('pg_showtransactions_month_height',300),
  );
  $form['overview_month']['pg_showtransactions_month_width'] = array (
    '#type'          => 'textfield',
    '#title'         => t('Width'),
    '#description'   => t('Please enter width of chart'),
    '#default_value' => variable_get('pg_showtransactions_month_width',500),
  );
  $form['overview_month']['pg_showtransactions_month_bgcolor'] = array (
    '#type'          => 'textfield',
    '#title'         => t('Background color'),
    '#description'   => t('Please enter code of background color'),
    '#default_value' => variable_get('pg_showtransactions_month_bgcolor','#FFFFFF'),
  );


  return system_settings_form($form);
}


function pg_showtransactions_yearchart() {

  $chart['height']  = variable_get('pg_showtransactions_year_height',300);
  $chart['width']   = variable_get('pg_showtransactions_year_width',500);
  $chart['bgcolor'] = variable_get('pg_showtransactions_year_bgcolor','#FFFFFF');
  $cur_year=date('Y');
  $from_year=$cur_year-variable_get('pg_showtransactions_year_count',4);
  $sql="SELECT year,status,sum(count) FROM {pg_showtransactions_stats} WHERE year >= '%d' AND year <= '%d' GROUP BY year,status";
  $result = db_query($sql,$from_year,$cur_year);
  $headers=array();
  while ($row = db_fetch_object($result)){
     $ret=array_keys($headers,$row->year);
     if(empty($ret)){
       $headers[]=$row->year;
     }
     $status[$row->status][$row->year]=$row->{'sum(count)'};
  }

  $chart['chart_x_labels']=$headers;
  $chart['chart_x_legend']=t('Year');
  $chart['chart_y_legend']=t('Type\'s count');
  $chart['max_y']=5;
  $chart['chart_type'] = array ();
  $chart['chart_title'] = array ();
  foreach($status as $st => $years){
    $row=array();
    $chart['chart_title'][]=pgapi_get_status($st);
    foreach($headers as $year){
      if(isset($years[$year])){
        $row[]=$years[$year];
        if($years[$year] > $chart['max_y']){
          $chart['max_y']=$years[$year];
        }
      }else{
          $row[]=0;
      }
    }
   $chart['chart_data'][]=$row;
   $chart['chart_type'][] = variable_get('pg_showtransactions_year_chart','bar_glass');
  }
  $output =  theme('pg_showtransactions_chart',t('Years stat'),$chart);
  return $output;
}

function pg_showtransactions_monthchart() {

  $chart['height']  = variable_get('pg_showtransactions_month_height',300);
  $chart['width']   = variable_get('pg_showtransactions_month_width',500);
  $chart['bgcolor'] = variable_get('pg_showtransactions_month_bgcolor','#FFFFFF');
  $cur_year=date('Y');
  $cur_month=date('m');
  $from_month=$cur_month-variable_get('pg_showtransactions_month_count',4);
  if($from_month < 0){
   $from_month  = 0;
  }
  $sql="SELECT month,status,sum(count) FROM {pg_showtransactions_stats} WHERE year = '%d' AND month >= '%d' AND month <= '%d' GROUP BY month,status";
  $result = db_query($sql,$cur_year,$from_month,$cur_month);
  $headers=array();
  $headers2=array();
  while ($row = db_fetch_object($result)){
     $ret=array_keys($headers2,$row->month);
     if(empty($ret)){
       $headers[]=date("M", mktime(0, 0, 0, $row->month, 1, 98));
       $headers2[]=$row->month;
     }
     $status[$row->status][$row->month]=$row->{'sum(count)'};
  }
  $chart['chart_x_labels']=$headers;
  $chart['chart_x_legend']=t('Month');
  $chart['chart_y_legend']=t('Type\'s count');
  $chart['max_y']=5;
  $chart['chart_type'] = array ();
  $chart['chart_title'] = array ();
  foreach($status as $st => $months){
    $chart['chart_title'][]=pgapi_get_status($st);
    $row=array();
    foreach($headers2 as $month){
      if(isset($months[$month])){
        $row[]=$months[$month];
        if($months[$month] > $chart['max_y']){
          $chart['max_y']=$months[$month];
        }

      }else{
          $row[]=0;
      }
    }
   $chart['chart_data'][]=$row;
   $chart['chart_type'][] = variable_get('pg_showtransactions_month_chart','bar_glass');

  }
  $output =  theme('pg_showtransactions_chart',t('Month stats'),$chart);
  return $output;
}

function pg_showtransactions_yearchart_money() {

  $chart['height']  = variable_get('pg_showtransactions_year_height',300);
  $chart['width']   = variable_get('pg_showtransactions_year_width',500);
  $chart['bgcolor'] = variable_get('pg_showtransactions_year_bgcolor','#FFFFFF');
  $chart['chart_type'] = variable_get('pg_showtransactions_year_chart','bar_glass');
  $cur_year=date('Y');
  $from_year=$cur_year-variable_get('pg_showtransactions_year_count',4);
  $sql="SELECT year,sum(amount) FROM {pg_showtransactions_amount} WHERE year >= '%d' AND year <= '%d' GROUP BY year";
  $result = db_query($sql,$from_year,$cur_year);
  $headers=array();
  while ($row = db_fetch_object($result)){
     if(!array_search($row->year,$headers)){
       $headers[]=$row->year;
     }
     $status[$row->status][$row->year]=$row->{'sum(amount)'};
  }
  $chart['chart_data'] = array ();
  $chart['chart_x_labels']=$headers;
  foreach($status as $st => $years){
    $row=array(t('Money'));
    foreach($headers as $year){
      if($year=='') continue;
      if(isset($years[$year])){
        $row[]=$years[$year];
      }else{
          $row[]=0;
      }
    }
   $chart['chart_data'][]=$row;
  }
  $output =  theme('pg_showtransactions_chart',t('Years money'),$chart);
  return $output;
}

function pg_showtransactions_monthchart_money() {

  $chart['height']  = variable_get('pg_showtransactions_month_height',300);
  $chart['width']   = variable_get('pg_showtransactions_month_width',500);
  $chart['bgcolor'] = variable_get('pg_showtransactions_month_bgcolor','#FFFFFF');
  $chart['chart_type'] = variable_get('pg_showtransactions_month_chart','bar_glass');
  $cur_year=date('Y');
  $cur_month=date('m');
  $from_month=$cur_month-variable_get('pg_showtransactions_month_count',4);
  if($from_month < 0){
   $from_month  = 0;
  }
  $sql="SELECT month,sum(amount) FROM {pg_showtransactions_amount} WHERE year = '%d' AND month >= '%d' AND month <= '%d' GROUP BY month";
  $result = db_query($sql,$cur_year,$from_month,$cur_month);
  $headers=array();
  while ($row = db_fetch_object($result)){
     if(!array_search($row->month,$headers)){
       $headers[]=$row->month;
     }
     $status[$row->status][$row->month]=$row->{'sum(amount)'};
  }
  $chart['chart_data'] = array ();
  $chart['chart_type'] = array();
  $chart['chart_x_labels']=$headers;
  foreach($status as $st => $months){
    $row=array(t('Money'));
    foreach($headers as $month){
      if($month=='') continue;
      if(isset($months[$month])){
        $row[]=$months[$month];
      }else{
          $row[]=0;
      }
    }
   $chart['chart_data'][]=$row;
   $chart['chart_type'][]='bar';
  }
  print_r($chart);
  $output =  theme('pg_showtransactions_chart',t('Month amount'),$chart);
  return $output;
}
