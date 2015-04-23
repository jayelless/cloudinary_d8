<?php

/**
 * @file
 * File for the Cloudinary transformation convert features.
 */

/**
 * Convert image effect cloudinary_crop to cloudinary style.
 */
function cloudinary_transformation_cloudinary_crop($effect, $exist_effect, $resource) {
  $data = cloudinary_prepare_transformation($effect['data'], FALSE);
  if (isset($data['multiple'])) {
    unset($data['multiple']);
    $new_data = array('type' => CLOUDINARY_STREAM_WRAPPER_TRANSFORMATION_MULTIPLE, 'data' => array());
    foreach ($data as $d) {
      $new_data['data'][] = cloudinary_transformation_image($d);
    }
    return $new_data;
  }
  return cloudinary_transformation_image($data);
}
