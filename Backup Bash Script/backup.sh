#!/bin/bash

backup_dir="$HOME/backup"
txt_files=$(sudo find /Users/jaskaransingh -name "*.txt")

mkdir -p "$backup_dir/cb"
mkdir -p "$backup_dir/ib"

while true; do
  # STEP 1
  timestamp=$(date +"%a %d %b %Y %r %Z")
  cb_tar_file="cb$(date +"%Y%m%d%H%M%S").tar"
  tar -cf "$backup_dir/cb/$cb_tar_file" $txt_files
  echo "${timestamp} $cb_tar_file was created" >> "$backup_dir/backup.log"

  # STEP 2
  sleep 120
  timestamp=$(date +"%a %d %b %Y %r %Z")
  ib_tar_file=""
  modified_files=$(sudo find /Users/jaskaransingh -name "*.txt" -newer "$backup_dir/cb/$cb_tar_file")
  if [ -n "$modified_files" ]; then
    ib_tar_file="ib$(date +"%Y%m%d%H%M%S").tar"
    tar -cf "$backup_dir/ib/$ib_tar_file" $modified_files
    echo "${timestamp} $ib_tar_file was created" >> "$backup_dir/backup.log"
  else
    echo "${timestamp} No changes-Incremental backup was not created" >> "$backup_dir/backup.log"
  fi

  # STEP 3
  sleep 120
  timestamp=$(date +"%a %d %b %Y %r %Z")
  ib_tar_file=""
  modified_files=$(sudo find /Users/jaskaransingh -name "*.txt" -newer "$backup_dir/ib/$ib_tar_file")
  if [ -n "$modified_files" ]; then
    ib_tar_file="ib$(date +"%Y%m%d%H%M%S").tar"
    tar -cf "$backup_dir/ib/$ib_tar_file" $modified_files
    echo "${timestamp} $ib_tar_file was created" >> "$backup_dir/backup.log"
  else
    echo "${timestamp} No changes-Incremental backup was not created" >> "$backup_dir/backup.log"
  fi

  # STEP 4
  sleep 120
  timestamp=$(date +"%a %d %b %Y %r %Z")
  ib_tar_file=""
  modified_files=$(sudo find /Users/jaskaransingh -name "*.txt" -newer "$backup_dir/ib/$ib_tar_file")
  if [ -n "$modified_files" ]; then
    ib_tar_file="ib$(date +"%Y%m%d%H%M%S").tar"
    tar -cf "$backup_dir/ib/$ib_tar_file" $modified_files
    echo "${timestamp} $ib_tar_file was created" >> "$backup_dir/backup.log"
  else
    echo "${timestamp} No changes-Incremental backup was not created" >> "$backup_dir/backup.log"
  fi

 

done

