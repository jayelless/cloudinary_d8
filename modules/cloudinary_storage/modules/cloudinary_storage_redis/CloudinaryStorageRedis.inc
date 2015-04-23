<?php

/**
 * @file
 * Redis storage implementation for uploaded Cloudinary files.
 */

/**
 * Implements cloudinary storage with redis.
 */
class CloudinaryStorageRedis extends CloudinaryStorage {
  /**
   * A current Redis connection.
   */
  protected $redis;

  /**
   * Object constructor.
   */
  public function __construct($resource = NULL, $loaded = TRUE) {
    // Init redis connection.
    $class = Redis_Client::getClass(Redis_Client::REDIS_IMPL_CACHE);
    $this->redis = new $class('cloudinary_storage_redis');
    parent::__construct($resource, $loaded);
  }

  /**
   * Create or update cloudinary file resource into redis.
   */
  protected function save($resource) {
    if (isset($resource['public_id'])) {
      $this->redis->set($resource['public_id'], $resource);
    }
  }

  /**
   * Delete cloudinary file resource from redis.
   */
  protected function delete($public_id) {
    $this->redis->clear($public_id);
  }

  /**
   * Delete cloudinary folder resource from redis.
   */
  protected function deleteFolder($public_id) {
    // Only remove file and folder resource in this folder.
    // Cloudinary can not delete folder, parent folder update is not necessary.
    $this->redis->clear($public_id, TRUE);
  }

  /**
   * Load cloudinary file resource from redis.
   */
  protected function load($public_id) {
    $resource = array();
    $cache = $this->redis->get($public_id);

    if ($cache && !empty($cache->data)) {
      $resource = (array) $cache->data;
    }

    return $resource;
  }

}
