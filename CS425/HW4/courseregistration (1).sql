PGDMP         4        	        y            postgres    11.12    11.12     .           0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                       false            /           0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                       false            0           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                       false            1           1262    13012    postgres    DATABASE     ?   CREATE DATABASE postgres WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'Chinese (Simplified)_China.936' LC_CTYPE = 'Chinese (Simplified)_China.936';
    DROP DATABASE postgres;
             postgres    false            2           0    0    DATABASE postgres    COMMENT     N   COMMENT ON DATABASE postgres IS 'default administrative connection database';
                  postgres    false    2865                        3079    16384 	   adminpack 	   EXTENSION     A   CREATE EXTENSION IF NOT EXISTS adminpack WITH SCHEMA pg_catalog;
    DROP EXTENSION adminpack;
                  false            3           0    0    EXTENSION adminpack    COMMENT     M   COMMENT ON EXTENSION adminpack IS 'administrative functions for PostgreSQL';
                       false    1            ?            1259    16434    Registration    TABLE     ?   CREATE TABLE public."Registration" (
    "Registration ID" integer NOT NULL,
    "Type" character varying NOT NULL,
    "StudentID" integer NOT NULL,
    "SectionID" integer NOT NULL
);
 "   DROP TABLE public."Registration";
       public         postgres    false            ?            1259    16423    Section    TABLE     ?   CREATE TABLE public."Section" (
    "SectionID" integer NOT NULL,
    "Number" integer NOT NULL,
    "Classroom" character varying NOT NULL,
    "Date/Time" date NOT NULL,
    "CourseID" integer NOT NULL,
    "InstructorID" integer NOT NULL
);
    DROP TABLE public."Section";
       public         postgres    false            ?            1259    16415    college    TABLE     ?   CREATE TABLE public.college (
    collegeid integer NOT NULL,
    collegename character varying NOT NULL,
    address character varying,
    telephone integer
);
    DROP TABLE public.college;
       public         postgres    false            ?            1259    16513    course    TABLE     ?   CREATE TABLE public.course (
    courseid integer NOT NULL,
    credit integer NOT NULL,
    coursename character varying(50) NOT NULL
);
    DROP TABLE public.course;
       public         postgres    false            ?            1259    16535 
   department    TABLE     ?   CREATE TABLE public.department (
    departmentid character varying(20) NOT NULL,
    deptname character varying(50) NOT NULL,
    collegeid integer NOT NULL
);
    DROP TABLE public.department;
       public         postgres    false            ?            1259    16447 
   instructor    TABLE     ?   CREATE TABLE public.instructor (
    instructorid integer NOT NULL,
    name character varying(20) NOT NULL,
    age integer NOT NULL
);
    DROP TABLE public.instructor;
       public         postgres    false            ?            1259    16530    student    TABLE     z   CREATE TABLE public.student (
    studentid integer NOT NULL,
    name character varying(50) NOT NULL,
    age integer
);
    DROP TABLE public.student;
       public         postgres    false            ?            1259    16540    student2    TABLE     ?   CREATE TABLE public.student2 (
    studentid integer NOT NULL,
    name character varying(50) NOT NULL,
    age integer NOT NULL
);
    DROP TABLE public.student2;
       public         postgres    false            &          0    16434    Registration 
   TABLE DATA               ]   COPY public."Registration" ("Registration ID", "Type", "StudentID", "SectionID") FROM stdin;
    public       postgres    false    199   ?       %          0    16423    Section 
   TABLE DATA               p   COPY public."Section" ("SectionID", "Number", "Classroom", "Date/Time", "CourseID", "InstructorID") FROM stdin;
    public       postgres    false    198            $          0    16415    college 
   TABLE DATA               M   COPY public.college (collegeid, collegename, address, telephone) FROM stdin;
    public       postgres    false    197           (          0    16513    course 
   TABLE DATA               >   COPY public.course (courseid, credit, coursename) FROM stdin;
    public       postgres    false    201   y        *          0    16535 
   department 
   TABLE DATA               G   COPY public.department (departmentid, deptname, collegeid) FROM stdin;
    public       postgres    false    203   <!       '          0    16447 
   instructor 
   TABLE DATA               =   COPY public.instructor (instructorid, name, age) FROM stdin;
    public       postgres    false    200   ?!       )          0    16530    student 
   TABLE DATA               7   COPY public.student (studentid, name, age) FROM stdin;
    public       postgres    false    202   @"       +          0    16540    student2 
   TABLE DATA               8   COPY public.student2 (studentid, name, age) FROM stdin;
    public       postgres    false    204   ?"       ?
           2606    16422    college College_pkey 
   CONSTRAINT     [   ALTER TABLE ONLY public.college
    ADD CONSTRAINT "College_pkey" PRIMARY KEY (collegeid);
 @   ALTER TABLE ONLY public.college DROP CONSTRAINT "College_pkey";
       public         postgres    false    197            ?
           2606    16451    instructor Instructor_pkey 
   CONSTRAINT     d   ALTER TABLE ONLY public.instructor
    ADD CONSTRAINT "Instructor_pkey" PRIMARY KEY (instructorid);
 F   ALTER TABLE ONLY public.instructor DROP CONSTRAINT "Instructor_pkey";
       public         postgres    false    200            ?
           2606    16441    Registration Registration_pkey 
   CONSTRAINT     i   ALTER TABLE ONLY public."Registration"
    ADD CONSTRAINT "Registration_pkey" PRIMARY KEY ("StudentID");
 L   ALTER TABLE ONLY public."Registration" DROP CONSTRAINT "Registration_pkey";
       public         postgres    false    199            ?
           2606    16430    Section Section_pkey 
   CONSTRAINT     _   ALTER TABLE ONLY public."Section"
    ADD CONSTRAINT "Section_pkey" PRIMARY KEY ("SectionID");
 B   ALTER TABLE ONLY public."Section" DROP CONSTRAINT "Section_pkey";
       public         postgres    false    198            ?
           2606    16517    course course_pkey 
   CONSTRAINT     V   ALTER TABLE ONLY public.course
    ADD CONSTRAINT course_pkey PRIMARY KEY (courseid);
 <   ALTER TABLE ONLY public.course DROP CONSTRAINT course_pkey;
       public         postgres    false    201            ?
           2606    16539    department department_pkey1 
   CONSTRAINT     c   ALTER TABLE ONLY public.department
    ADD CONSTRAINT department_pkey1 PRIMARY KEY (departmentid);
 E   ALTER TABLE ONLY public.department DROP CONSTRAINT department_pkey1;
       public         postgres    false    203            ?
           2606    16544    student2 student2_pkey 
   CONSTRAINT     [   ALTER TABLE ONLY public.student2
    ADD CONSTRAINT student2_pkey PRIMARY KEY (studentid);
 @   ALTER TABLE ONLY public.student2 DROP CONSTRAINT student2_pkey;
       public         postgres    false    204            ?
           2606    16534    student student_pkey 
   CONSTRAINT     Y   ALTER TABLE ONLY public.student
    ADD CONSTRAINT student_pkey PRIMARY KEY (studentid);
 >   ALTER TABLE ONLY public.student DROP CONSTRAINT student_pkey;
       public         postgres    false    202            &      x?????? ? ?      %      x?????? ? ?      $   L   x?340?t???IMOU?OSp??-(-??K???".C#diל?䒢????ļ???"׼?̼??"??=... u?[      (   ?   x?m?;?@Dk?)|D??)#??JJ?%1??`?N?!?????`;?+???^?-?T?E?^?)?D$1?1P4?jj??\?
7֌?P?k`???:??l?v???ۨ4~?!??N?մX???W??N1'????&9??2 _pԍ$???7?S<?????S|???E????9???R1      *   s   x?e?;?0@g?9?ܠ?<t?Ԏ,?1??ԎB?=Ѕ??}"A??>??@??D?Z??|?????*????ޔS???&?{?SW??
o????x??Z??-̩o???q??	??4-{      '   q   x???
?@??{񲛿R??J[??X? ?A?o??3g/??!??%Ŋ?5EhC?\?m/ː?-????p?????6??????,'_ #Gk??N??-?4??#??=?      )   k   x?5̱
?@E????	;?F??"?X?Y??L?
?6?????6???-A*V?!#??l1[?vX????b?}ꑡ-{,??/??)???<G?<?xj.%?}#??L      +   F   x?3?t+?LM?4??2????4??2????42????M,*???-9}2?RA????%?E ?=... 5??     