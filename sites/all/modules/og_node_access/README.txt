# OG Node Access

This module extends OG Access with helper functionality to help in wrangling
access to Organic Group posts. The current version has no affect on site 
functionality unless a developer makes use of its code.

## Features

This module only provides one piece of functionality at this time: a forked
version of the node access rebuild process that allows developers to target
specific Organic Groups for rebuild.

## Usage

Use the function `og_node_access_needs_rebuild($group_nid)` and the specified
group will be tagged for an access rebuild.
